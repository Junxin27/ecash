#pragma once

#include <string>
#include <unordered_set>

#include "crypto/bbs.hpp"
#include "crypto/bbs_generators.hpp"
#include "wallet/coin.hpp"
#include "protocol/withdraw_request.hpp"
#include "protocol/true_blind.hpp"

class Bank {
public:
    Bank();

    void hello() const;

    const crypto::BBSPublicKey& public_key() const;
    const crypto::BBSGenerators& generators() const;

    bool issue_coin(wallet::Coin& coin) const;
    bool issue_coin_blind(wallet::Coin& coin, const protocol::WithdrawRequest& req) const;
    protocol::TrueBlindResponse issue_true_blind(const protocol::TrueBlindRequest& req) const;
    bool deposit_coin(const wallet::Coin& coin);

private:
    crypto::BBSSecretKey sk_;
    crypto::BBSPublicKey pk_;
    crypto::BBSGenerators gens_;

    std::unordered_set<std::string> spent_serials_;

    bool validate_blind_request(const protocol::WithdrawRequest& req) const;
    std::string serial_to_hex(const BIG_384_58 serial) const;
    std::string commitment_to_hex(const ECP_BLS381& point) const;
};