#include <iostream>
#include "protocol/payment_transcript.hpp"
#include "wallet/coin.hpp"

namespace protocol {

void print_payment_transcript(const PaymentTranscript& tx) {
    std::cout << "\n=== Payment Transcript ===\n";
    std::cout << "Merchant ID: " << tx.merchant_id << "\n";
    std::cout << "Coin index: " << tx.coin_index << "\n";
    std::cout << "Merchant verification: " << (tx.merchant_verified ? "PASS" : "FAIL") << "\n";
    std::cout << "Bank deposit status: " << (tx.bank_accepted ? "ACCEPTED" : "REJECTED") << "\n";
    wallet::print_coin(tx.coin);
}

}