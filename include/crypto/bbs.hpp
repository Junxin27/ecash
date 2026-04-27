#pragma once

extern "C" {
#include "pair_BLS381.h"
#include "ecp_BLS381.h"
#include "ecp2_BLS381.h"
#include "fp12_BLS381.h"
#include "randapi.h"
}

namespace crypto {

struct BBSSecretKey {
    BIG_384_58 x;
};

struct BBSPublicKey {
    ECP2_BLS381 w;
};

void bbs_generate_keypair(BBSSecretKey& sk, BBSPublicKey& pk);
void bbs_print_public_key(const BBSPublicKey& pk);

}