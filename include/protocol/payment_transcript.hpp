#pragma once

#include <string>
#include "wallet/coin.hpp"

namespace protocol {

struct PaymentTranscript {
    std::size_t coin_index = 0;
    std::string merchant_id;
    wallet::Coin coin;
    bool merchant_verified = false;
    bool bank_accepted = false;
};

void print_payment_transcript(const PaymentTranscript& tx);

}