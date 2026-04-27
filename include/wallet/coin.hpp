#pragma once

#include "crypto/bbs_signature.hpp"

extern "C" {
#include "big_384_58.h"
}

namespace wallet {

struct Coin {
    BIG_384_58 serial;
    BIG_384_58 randomness;
    BIG_384_58 value;

    BIG_384_58 blind_factor;

    ECP_BLS381 commitment;
    ECP_BLS381 blinded_commitment;

    BIG_384_58 spend_serial;
    BIG_384_58 spend_randomness;

    crypto::BBSSignature signature;
    bool spent = false;
    bool blind_issued = false;
};

void init_coin(Coin& coin);
void print_coin(const Coin& coin);

}