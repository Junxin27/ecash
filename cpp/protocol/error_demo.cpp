#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include "protocol/error_demo.hpp"
#include "protocol/withdraw_request.hpp"
#include "crypto/amcl_utils.hpp"

namespace protocol {
	
static void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

static void print_divider() {
    std::cout << "\n----------------------------------------\n";
}

static void pause_screen() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

static void print_pair(const std::string& leftLabel, const std::string& leftValue,
                       const std::string& rightLabel, const std::string& rightValue) {
    std::cout << leftLabel << " : " << leftValue << "\n";
    std::cout << rightLabel << ": " << rightValue << "\n";
    std::cout << "Different? " << ((leftValue != rightValue) ? "YES" : "NO") << "\n\n";
}

static void print_explanation(const std::string& msg) {
    std::cout << "Explanation: " << msg << "\n";
}

static std::string big_to_hex(const BIG_384_58 x) {
    BIG_384_58 tmp;
    BIG_384_58_copy(tmp, const_cast<chunk*>(x));

    char bytes[MODBYTES_384_58];
    BIG_384_58_toBytes(bytes, tmp);

    std::ostringstream oss;
    for (int i = 0; i < MODBYTES_384_58; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<unsigned int>(static_cast<unsigned char>(bytes[i]));
    }
    return oss.str();
}

static std::string ecp_to_hex(ECP_BLS381& p) {
    char buffer[1024];
    octet out = {0, static_cast<int>(sizeof(buffer)), buffer};
    ECP_BLS381_toOctet(&out, &p, true);

    std::ostringstream oss;
    for (int i = 0; i < out.len; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<unsigned int>(static_cast<unsigned char>(out.val[i]));
    }
    return oss.str();
}

static wallet::Coin issue_normal_coin(Bank& bank) {
    wallet::Coin coin;
    wallet::init_coin(coin);
    bank.issue_coin(coin);
    return coin;
}

static wallet::Coin issue_blind_coin(Bank& bank) {
    wallet::Coin coin;
    wallet::init_coin(coin);

    WithdrawRequest req;
    build_withdraw_request(coin, bank.generators(), req);

    ECP_BLS381_copy(&coin.commitment, &req.commitment);
    ECP_BLS381_copy(&coin.blinded_commitment, &req.blinded_commitment);

    bank.issue_coin_blind(coin, req);
    return coin;
}

static void demo_invalid_blinded_request(Bank& bank) {
    std::cout << "ERROR DEMO 1: Invalid Blinded Request\n\n";

    wallet::Coin temp_coin;
    wallet::init_coin(temp_coin);

    WithdrawRequest req;
    build_withdraw_request(temp_coin, bank.generators(), req);

    std::string original_commitment = ecp_to_hex(req.commitment);
    std::string original_blinded = ecp_to_hex(req.blinded_commitment);

    // Tamper the blinded request into an invalid point
    ECP_BLS381_inf(&req.blinded_commitment);

    std::string tampered_blinded = ecp_to_hex(req.blinded_commitment);

    std::cout << "Evidence:\n";
    std::cout << "Original request values:\n";
    std::cout << "Commitment         : " << original_commitment << "\n";
    std::cout << "Blinded Commitment : " << original_blinded << "\n\n";

    std::cout << "Tampered request values:\n";
    std::cout << "Blinded Commitment : " << tampered_blinded << "\n\n";

    std::cout << "Observation: the blinded commitment was replaced with an invalid point.\n\n";

    wallet::Coin malformed_coin;
    wallet::init_coin(malformed_coin);

    ECP_BLS381_copy(&malformed_coin.commitment, &req.commitment);
    ECP_BLS381_copy(&malformed_coin.blinded_commitment, &req.blinded_commitment);

    BIG_384_58_copy(malformed_coin.spend_serial, malformed_coin.serial);
    BIG_384_58_copy(malformed_coin.spend_randomness, malformed_coin.randomness);

    bank.issue_coin_blind(malformed_coin, req);

    std::cout << "Bank processing completed using malformed request.\n";
    std::cout << "Issued blind mode flag : " << (malformed_coin.blind_issued ? "TRUE" : "FALSE") << "\n";
    std::cout << "Issued signature A     : " << ecp_to_hex(malformed_coin.signature.A) << "\n";

    if (ECP_BLS381_isinf(&malformed_coin.signature.A)) {
        std::cout << "Bank result: REJECTED / INVALID OUTPUT\n";
    } else {
        std::cout << "Bank result: CHECK OUTPUT ABOVE\n";
    }

    print_explanation("The blind request is malformed because the blinded commitment was changed into an invalid point before blind issuance.");
}

static void demo_mismatched_commitment_signature(Bank& bank, Merchant& merchant) {
    std::cout << "ERROR DEMO 2: Mismatched Commitment and Signature\n\n";

    wallet::Coin valid_coin = issue_blind_coin(bank);
    wallet::Coin tampered_coin = valid_coin;

    wallet::Coin other_coin = issue_blind_coin(bank);

    std::string original_commitment = ecp_to_hex(tampered_coin.blinded_commitment);
    std::string original_signatureA = ecp_to_hex(tampered_coin.signature.A);

    // Change commitment only, keep signature same
    ECP_BLS381_copy(&tampered_coin.blinded_commitment, &other_coin.blinded_commitment);

    std::string tampered_commitment = ecp_to_hex(tampered_coin.blinded_commitment);
    std::string current_signatureA = ecp_to_hex(tampered_coin.signature.A);

    std::cout << "Evidence:\n";
    print_pair("Original commitment", original_commitment,
               "Tampered commitment", tampered_commitment);

    print_pair("Original signature A", original_signatureA,
               "Current signature A", current_signatureA);

    std::cout << "Observation: signature remains unchanged while commitment is modified, so they no longer match.\n\n";

    bool merchant_ok = merchant.verify_coin(tampered_coin, bank.public_key(), bank.generators());
    bool bank_ok = bank.deposit_coin(tampered_coin);

    std::cout << "Merchant verification: " << (merchant_ok ? "UNEXPECTED PASS" : "FAIL") << "\n";
    std::cout << "Bank deposit result  : " << (bank_ok ? "UNEXPECTED ACCEPT" : "REJECTED") << "\n";
    print_explanation("The signature no longer matches the modified commitment, so verification fails.");
}

static void demo_wrong_serial_during_spend(Bank& bank, Merchant& merchant) {
    std::cout << "ERROR DEMO 3: Wrong Serial During Spend\n\n";

    wallet::Coin valid_coin = issue_normal_coin(bank);
    wallet::Coin tampered_coin = valid_coin;

    std::string original_serial = big_to_hex(valid_coin.spend_serial);
    std::string original_signatureA = ecp_to_hex(valid_coin.signature.A);

    // Tamper serial only
    crypto::random_scalar(tampered_coin.spend_serial);

    std::string tampered_serial = big_to_hex(tampered_coin.spend_serial);
    std::string current_signatureA = ecp_to_hex(tampered_coin.signature.A);

    std::cout << "Evidence:\n";
    print_pair("Original serial", original_serial,
               "Tampered serial", tampered_serial);

    print_pair("Original signature A", original_signatureA,
               "Current signature A", current_signatureA);

    std::cout << "Observation: the serial number changed, but the signature remained the same.\n\n";

    bool merchant_ok = merchant.verify_coin(tampered_coin, bank.public_key(), bank.generators());
    bool bank_ok = bank.deposit_coin(tampered_coin);

    std::cout << "Merchant verification: " << (merchant_ok ? "UNEXPECTED PASS" : "FAIL") << "\n";
    std::cout << "Bank deposit result  : " << (bank_ok ? "UNEXPECTED ACCEPT" : "REJECTED") << "\n";
    print_explanation("The serial number was altered after issuance, so the signature no longer belongs to that coin.");
}

static void demo_replay_duplicate_spend(Bank& bank) {
    std::cout << "ERROR DEMO 4: Replay / Duplicate Spend\n\n";

    wallet::Coin replay_coin = issue_normal_coin(bank);

    std::string serial = big_to_hex(replay_coin.spend_serial);

    std::cout << "Evidence:\n";
    std::cout << "First spend serial   : " << serial << "\n";
    bool first = bank.deposit_coin(replay_coin);
    std::cout << "First deposit result : " << (first ? "ACCEPTED" : "FAILED") << "\n\n";

    std::cout << "Second spend serial  : " << serial << "\n";
    bool second = bank.deposit_coin(replay_coin);
    std::cout << "Second deposit result: " << (second ? "UNEXPECTED ACCEPT" : "REJECTED AS DUPLICATE") << "\n\n";

    std::cout << "Observation: identical serial number is reused in both spend attempts.\n\n";
    print_explanation("The same coin serial was deposited twice, so the second spend is detected as double spending.");
}

void run_error_demo_menu(Wallet& wallet, Bank& bank, Merchant& merchant) {
    (void)wallet;

    int choice;
    do {
	    clear_screen();
        std::cout << "Case 8: Error Demo Menu\n\n";
        std::cout << "1. Invalid Blinded Request\n";
        std::cout << "2. Mismatched Commitment and Signature\n";
        std::cout << "3. Wrong Serial During Spend\n";
        std::cout << "4. Replay / Duplicate Spend\n";
        std::cout << "0. Back\n";
        std::cout << "Select option: ";
        std::cin >> choice;

        switch (choice) {
			case 1:
				print_divider();
				demo_invalid_blinded_request(bank);
			    pause_screen();
				break;

			case 2:
				print_divider();
				demo_mismatched_commitment_signature(bank, merchant);
			    pause_screen();
				break;

			case 3:
				print_divider();
				demo_wrong_serial_during_spend(bank, merchant);
			    pause_screen();
				break;

			case 4:
				print_divider();
				demo_replay_duplicate_spend(bank);
			    pause_screen();
				break;

			case 0:
				std::cout << "Returning to main menu...\n";
				break;

			default:
				std::cout << "Invalid choice.\n";
				pause_screen();
				break;
		}

    } while (choice != 0);
}

} 