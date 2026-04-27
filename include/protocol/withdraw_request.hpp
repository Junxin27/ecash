#pragma once

#include "wallet/coin.hpp"
#include "crypto/bbs_generators.hpp"

namespace protocol {

struct WithdrawRequest {
    ECP_BLS381 commitment;
    ECP_BLS381 blinded_commitment;
};

void build_withdraw_request(
    const wallet::Coin& coin,
    const crypto::BBSGenerators& gens,
    WithdrawRequest& req
);

void print_withdraw_request(const WithdrawRequest& req);

}