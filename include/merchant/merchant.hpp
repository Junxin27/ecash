#pragma once

#include <vector>
#include "crypto/bbs.hpp"
#include "crypto/bbs_generators.hpp"
#include "wallet/coin.hpp"
#include "merchant/merchant_record.hpp"

class Merchant {
public:
    void hello() const;

    bool verify_coin(
        const wallet::Coin& coin,
        const crypto::BBSPublicKey& pk,
        const crypto::BBSGenerators& gens
    ) const;

    void add_record(const merchant::MerchantRecord& record);
    void print_records() const;
    std::size_t record_count() const;

private:
    std::vector<merchant::MerchantRecord> records_;
};