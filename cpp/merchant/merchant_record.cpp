#include <iostream>
#include "merchant/merchant_record.hpp"

namespace merchant {

void print_record(const MerchantRecord& record) {
    std::cout << "\n=== Merchant Record ===\n";
    std::cout << "Merchant ID: " << record.merchant_id << "\n";
    std::cout << "Coin index: " << record.coin_index << "\n";
    std::cout << "Verification passed: " << (record.verification_passed ? "YES" : "NO") << "\n";
    std::cout << "Deposit accepted: " << (record.deposit_accepted ? "YES" : "NO") << "\n";
    std::cout << "Status: " << record.status << "\n";
}

}