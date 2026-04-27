#pragma once

extern "C" {
#include "pair_BLS381.h"
#include "ecp_BLS381.h"
#include "ecp2_BLS381.h"
}

#include "crypto/bbs.hpp"
#include "crypto/bbs_generators.hpp"

namespace crypto {

struct BBSSignature {
    ECP_BLS381 A;
    BIG_384_58 e;
    BIG_384_58 s;
};

void bbs_sign_demo(
    const BBSSecretKey& sk,
    const BBSGenerators& gens,
    const BIG_384_58 m1,
    const BIG_384_58 m2,
    BBSSignature& sig
);

bool bbs_verify_demo(
    const BBSPublicKey& pk,
    const BBSGenerators& gens,
    const BIG_384_58 m1,
    const BIG_384_58 m2,
    const BBSSignature& sig
);

void bbs_print_signature(const BBSSignature& sig);

}