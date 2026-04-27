#include <iostream>
#include <limits>
#include <cstdlib>

#include "ui/dashboard.hpp"
#include "protocol/transaction_flow.hpp"
#include "protocol/payment_transcript.hpp"
#include "protocol/true_blind.hpp"
#include "protocol/error_demo.hpp"

namespace ui {

static std::size_t count_unspent(const Wallet& wallet) {
    std::size_t count = 0;
    for (std::size_t i = 0; i < wallet.coin_count(); ++i) {
        if (!wallet.is_coin_spent(i)) {
            ++count;
        }
    }
    return count;
}

static std::size_t count_spent(const Wallet& wallet) {
    std::size_t count = 0;
    for (std::size_t i = 0; i < wallet.coin_count(); ++i) {
        if (wallet.is_coin_spent(i)) {
            ++count;
        }
    }
    return count;
}

static void clear_bad_input() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static void clear_screen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

static void pause_screen() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

static void show_dashboard(const Wallet& wallet, const Merchant& merchant) {
    std::cout << "=============================================\n";
    std::cout << "         E-CASH SIMULATION DASHBOARD\n";
    std::cout << "=============================================\n";

    std::cout << "\n[ WALLET ]\n";
    std::cout << "Total coins : " << wallet.coin_count() << "\n";
    std::cout << "Unspent     : " << count_unspent(wallet) << "\n";
    std::cout << "Spent       : " << count_spent(wallet) << "\n";

    std::cout << "\n[ MERCHANT ]\n";
    std::cout << "Records     : " << merchant.record_count() << "\n";

    std::cout << "\n---------------------------------------------\n";
    std::cout << "1. Blind Withdraw 1 Coin\n";
    std::cout << "2. Withdraw 1 Coin\n";
    std::cout << "3. Show Wallet\n";
    std::cout << "4. Spend Selected Coin\n";
    std::cout << "5. Show Merchant Records\n";
    std::cout << "6. Show Bank Public Parameters\n";
    std::cout << "7. True Blind Demo\n";
    std::cout << "8. Error Demo\n";
    std::cout << "0. Exit\n";
    std::cout << "---------------------------------------------\n";
}

static void run_true_blind_demo(Bank& bank) {
    std::cout << "\n=== True Blind Flow Demo ===\n";

    wallet::Coin test_coin;
    wallet::init_coin(test_coin);

    protocol::TrueBlindRequest req;
    protocol::build_true_blind_request(test_coin, bank.generators(), req);

    protocol::TrueBlindResponse resp = bank.issue_true_blind(req);
    protocol::print_true_blind_demo(req, resp);

    protocol::UnblindedCoin unblinded;
    bool ok = protocol::unblind_signature(req, resp, unblinded);

    std::cout << "\nUnblinding result: " << (ok ? "SUCCESS" : "FAIL") << "\n";

    bool verify_ok = protocol::verify_unblinded_coin(unblinded, bank.public_key());
    std::cout << "Unblinded coin verification: " << (verify_ok ? "PASS" : "FAIL") << "\n";
}

void run_dashboard(Wallet& wallet, Bank& bank, Merchant& merchant) {
    int choice = -1;

    while (choice != 0) {
        clear_screen();
        show_dashboard(wallet, merchant);

        std::cout << "Select option: ";
        if (!(std::cin >> choice)) {
            clear_bad_input();
            std::cout << "Invalid input.\n";
            pause_screen();
            continue;
        }

        switch (choice) {
        case 1: {
            std::cout << "\n--- Blind Withdraw 1 Coin ---\n";
            protocol::blind_withdraw_phase(wallet, bank);
            pause_screen();
            break;
        }

        case 2: {
            std::cout << "\n--- Withdraw 1 Coin ---\n";
            protocol::withdraw_multiple_phase(wallet, bank);
            pause_screen();
            break;
        }

        case 3: {
            std::cout << "\n--- Wallet Contents ---\n";
            wallet.print_all_coins();
            pause_screen();
            break;
        }

        case 4: {
            if (!wallet.has_coins()) {
                std::cout << "No coins available in wallet.\n";
                pause_screen();
                break;
            }

            std::size_t index = 0;
            std::cout << "Enter coin index to spend: ";
            if (!(std::cin >> index)) {
                clear_bad_input();
                std::cout << "Invalid coin index.\n";
                pause_screen();
                break;
            }

            if (index >= wallet.coin_count()) {
                std::cout << "Coin index out of range.\n";
                pause_screen();
                break;
            }

            protocol::PaymentTranscript tx =
                protocol::execute_payment(wallet, bank, merchant, index, "Merchant-A");

            protocol::print_payment_transcript(tx);

            if (tx.bank_accepted) {
                std::cout << "\nAttempting second deposit of same coin...\n";
                const bool second_attempt = protocol::deposit_phase(wallet, bank, index);
                std::cout << "Second deposit result: "
                          << (second_attempt ? "SUCCESS" : "FAIL (double spend detected)") << "\n";
            } else {
                std::cout << "\nSecond deposit test skipped because first payment was not accepted.\n";
            }

            pause_screen();
            break;
        }

        case 5: {
            std::cout << "\n--- Merchant Records ---\n";
            merchant.print_records();
            pause_screen();
            break;
        }

        case 6: {
            std::cout << "\n--- Bank Public Parameters ---\n";
            crypto::bbs_print_public_key(bank.public_key());
            crypto::bbs_print_generators(bank.generators());
            pause_screen();
            break;
        }

        case 7: {
            run_true_blind_demo(bank);
            pause_screen();
            break;
        }

        case 8:
			protocol::run_error_demo_menu(wallet, bank, merchant);
			pause_screen();
			break;

        case 0:
            std::cout << "Exiting dashboard...\n";
            break;

        default:
            std::cout << "Invalid option.\n";
            pause_screen();
            break;
        }
    }
}

}