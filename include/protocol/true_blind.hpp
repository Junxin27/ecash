#pragma once

#include "wallet/coin.hpp"
#include "crypto/bbs_signature.hpp"
#include "crypto/bbs_generators.hpp"
#include "crypto/bbs.hpp"

namespace protocol {

struct TrueBlindRequest {
    ECP_BLS381 commitment;
    ECP_BLS381 blinded_commitment;
    BIG_384_58 blind_factor;
};

struct TrueBlindResponse {
    crypto::BBSSignature blinded_signature;
    bool issued = false;
};

struct UnblindedCoin {
    ECP_BLS381 commitment;
    crypto::BBSSignature signature;
    bool ready = false;
};

void build_true_blind_request(
    const wallet::Coin& coin,
    const crypto::BBSGenerators& gens,
    TrueBlindRequest& req
);

void print_true_blind_request(const TrueBlindRequest& req);
void print_true_blind_response(const TrueBlindResponse& resp);
void print_unblinded_coin(const UnblindedCoin& coin);

void print_true_blind_demo(
    const TrueBlindRequest& req,
    const TrueBlindResponse& resp
);

bool unblind_signature(
    const TrueBlindRequest& req,
    const TrueBlindResponse& resp,
    UnblindedCoin& coin
);

bool verify_unblinded_coin(
    const UnblindedCoin& coin,
    const crypto::BBSPublicKey& pk
);

}