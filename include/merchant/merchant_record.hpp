#pragma once

#include <string>

namespace merchant {

struct MerchantRecord {
    std::string merchant_id;
    std::size_t coin_index = 0;
    bool verification_passed = false;
    bool deposit_accepted = false;
    std::string status;
};

void print_record(const MerchantRecord& record);

}