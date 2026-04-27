#include <iostream>
#include "wallet/wallet.hpp"
#include "bank/bank.hpp"
#include "merchant/merchant.hpp"
#include "crypto/rust_bridge.hpp"
#include "crypto/pairing_test.hpp"
#include "crypto/amcl_utils.hpp"
#include "crypto/bbs.hpp"
#include "crypto/bbs_generators.hpp"
#include "crypto/bbs_signature.hpp"
#include "wallet/coin.hpp"
#include "protocol/transaction_flow.hpp"
#include "protocol/payment_transcript.hpp"

int main() {
    std::cout << "E-Cash FYP prototype starting...\n";

    crypto::init_rng();

    Wallet user_wallet;
    Bank bank;
    Merchant merchant;

    user_wallet.hello();
    bank.hello();
    merchant.hello();

    auto value = rust_bridge::random_u32();
    auto digest_first = rust_bridge::sha256_first_byte(value);

    std::cout << "Rust random value: " << value << "\n";
    std::cout << "Rust SHA-256 first byte: " << static_cast<int>(digest_first) << "\n";

    crypto::pairing_smoke_test();

    crypto::bbs_print_public_key(bank.public_key());
    crypto::bbs_print_generators(bank.generators());
  
    std::cout << "\n--- Blind Withdraw Demo ---\n";
    protocol::blind_withdraw_phase(user_wallet, bank);

    const std::size_t requested_coins = 3;
    const std::size_t selected_index = 1;

    std::size_t issued_count = protocol::withdraw_multiple_phase(user_wallet, bank, requested_coins);
    std::cout << "\nTotal issued coins: " << issued_count << "\n";

    std::cout << "Wallet coin count: " << user_wallet.coin_count() << "\n";
    user_wallet.print_all_coins();

    if (!user_wallet.has_coins()) {
        std::cout << "No coins available for spending.\n";
        crypto::cleanup_rng();
        std::cout << "Project setup is working.\n";
        return 0;
    }

    if (selected_index >= user_wallet.coin_count()) {
        std::cout << "Selected coin index is out of range.\n";
        crypto::cleanup_rng();
        std::cout << "Project setup is working.\n";
        return 0;
    }

    protocol::PaymentTranscript tx =
        protocol::execute_payment(user_wallet, bank, merchant, selected_index, "Merchant-A");

    protocol::print_payment_transcript(tx);
    merchant.print_records();

    bool second_attempt = protocol::deposit_phase(user_wallet, bank, selected_index);
    std::cout << "Second deposit result: "
              << (second_attempt ? "SUCCESS" : "FAIL (double spend detected)") << "\n";

    std::cout << "\nWallet state after deposit attempts:\n";
    user_wallet.print_all_coins();

    ECP_BLS381 g1;
    crypto::get_g1_generator(&g1);
    crypto::print_g1("G1 generator", &g1);

    crypto::cleanup_rng();

    std::cout << "Project setup is working.\n";
    return 0;
}