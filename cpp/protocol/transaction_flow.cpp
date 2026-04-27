#include <iostream>
#include <iomanip>
#include "protocol/transaction_flow.hpp"
#include "protocol/withdraw_request.hpp"

namespace protocol {

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

bool blind_withdraw_phase(Wallet& wallet, const Bank& bank) {
    wallet::Coin new_coin;
    wallet::init_coin(new_coin);

    protocol::WithdrawRequest req;
    protocol::build_withdraw_request(new_coin, bank.generators(), req);

    const bool issued = bank.issue_coin_blind(new_coin, req);
    std::cout << "Blind coin issuance: " << (issued ? "SUCCESS" : "FAIL") << "\n";
	
    ECP_BLS381_copy(&new_coin.commitment, &req.commitment);
    ECP_BLS381_copy(&new_coin.blinded_commitment, &req.blinded_commitment);

    protocol::print_withdraw_request(req);

    if (issued) {
        wallet.add_coin(new_coin);
    }

    return issued;
}

bool withdraw_multiple_phase(Wallet& wallet, const Bank& bank) {

    wallet::Coin new_coin;
    wallet::init_coin(new_coin);

    const bool issued = bank.issue_coin(new_coin);
    std::cout << "Coin issuance: " << (issued ? "SUCCESS" : "FAIL") << "\n";

    if (issued) {
        // ===== Additional Coin Details =====
        std::cout << "Serial Number : " << big_to_hex(new_coin.serial) << "\n";
        std::cout << "Randomness    : " << big_to_hex(new_coin.randomness) << "\n\n";

        // Signature (BBS+)
        std::cout << "Signature:\n";
        std::cout << "  A : " << ecp_to_hex(new_coin.signature.A) << "\n";
        std::cout << "  e : " << big_to_hex(new_coin.signature.e) << "\n";
        std::cout << "  s : " << big_to_hex(new_coin.signature.s) << "\n";

        wallet.add_coin(new_coin);
    }

    return issued;
}

bool spend_phase(const Wallet& wallet, const Bank& bank, const Merchant& merchant, std::size_t coin_index) {
    if (!wallet.has_coins() || coin_index >= wallet.coin_count()) {
        std::cout << "Spend phase: no valid coin available.\n";
        return false;
    }

    if (wallet.is_coin_spent(coin_index)) {
        std::cout << "Spend phase: selected coin is already marked as spent.\n";
        return false;
    }

    const wallet::Coin& selected_coin = wallet.get_coin(coin_index);

    std::cout << "\n[Spend] Using coin index: " << coin_index << "\n";

    const bool verified = merchant.verify_coin(
        selected_coin,
        bank.public_key(),
        bank.generators()
    );

    std::cout << "Merchant verification: " << (verified ? "PASS" : "FAIL") << "\n";
    return verified;
}

bool deposit_phase(Wallet& wallet, Bank& bank, std::size_t coin_index) {
    if (!wallet.has_coins() || coin_index >= wallet.coin_count()) {
        std::cout << "Deposit phase: no valid coin available.\n";
        return false;
    }

    if (wallet.is_coin_spent(coin_index)) {
        std::cout << "Deposit phase: selected coin is already marked as spent.\n";
        return false;
    }

    const wallet::Coin& selected_coin = wallet.get_coin(coin_index);

    std::cout << "[Deposit] Using coin index: " << coin_index << "\n";

    const bool accepted = bank.deposit_coin(selected_coin);
    std::cout << "Deposit attempt: " << (accepted ? "ACCEPTED" : "REJECTED") << "\n";

    if (accepted) {
        wallet.mark_coin_spent(coin_index);
    }

    return accepted;
}

PaymentTranscript execute_payment(
    Wallet& wallet,
    Bank& bank,
    Merchant& merchant,
    std::size_t coin_index,
    const std::string& merchant_id
) {
    PaymentTranscript tx;
    tx.coin_index = coin_index;
    tx.merchant_id = merchant_id;

    if (!wallet.has_coins() || coin_index >= wallet.coin_count()) {
        std::cout << "Execute payment: invalid coin index.\n";
        return tx;
    }

    tx.coin = wallet.get_coin(coin_index);

    tx.merchant_verified = spend_phase(wallet, bank, merchant, coin_index);

    if (!tx.merchant_verified) {
        tx.bank_accepted = false;
    } else {
        tx.bank_accepted = deposit_phase(wallet, bank, coin_index);
    }

    tx.coin = wallet.get_coin(coin_index);

    merchant::MerchantRecord record;
    record.merchant_id = merchant_id;
    record.coin_index = coin_index;
    record.verification_passed = tx.merchant_verified;
    record.deposit_accepted = tx.bank_accepted;

    if (!tx.merchant_verified) {
        record.status = "Verification failed";
    } else if (tx.bank_accepted) {
        record.status = "Payment accepted";
    } else {
        record.status = "Deposit rejected";
    }

    merchant.add_record(record);

    return tx;
}

}