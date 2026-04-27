#include <iostream>
#include "merchant/merchant.hpp"
#include "crypto/bbs_signature.hpp"
#include "crypto/bbs_commitment.hpp"

void Merchant::hello() const {
    std::cout << "Merchant module loaded.\n";
}

bool Merchant::verify_coin(
    const wallet::Coin& coin,
    const crypto::BBSPublicKey& pk,
    const crypto::BBSGenerators& gens
) const {
    if (coin.blind_issued) {
        return crypto::bbs_verify_commitment_demo(pk, coin.blinded_commitment, coin.signature);
    }

    return crypto::bbs_verify_demo(
        pk,
        gens,
        coin.spend_serial,
        coin.spend_randomness,
        coin.signature
    );
}

void Merchant::add_record(const merchant::MerchantRecord& record) {
    records_.push_back(record);
}

void Merchant::print_records() const {
    std::cout << "\nMerchant stored records: " << records_.size() << "\n";
    for (const auto& record : records_) {
        merchant::print_record(record);
    }
}

std::size_t Merchant::record_count() const {
    return records_.size();
}