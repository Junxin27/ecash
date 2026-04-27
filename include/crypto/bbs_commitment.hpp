#pragma once

extern "C" {
#include "pair_BLS381.h"
#include "ecp_BLS381.h"
#include "ecp2_BLS381.h"
}

#include "crypto/bbs.hpp"
#include "crypto/bbs_signature.hpp"

namespace crypto {

void bbs_sign_commitment_demo(
    const BBSSecretKey& sk,
    const ECP_BLS381& commitment,
    BBSSignature& sig
);

bool bbs_verify_commitment_demo(
    const BBSPublicKey& pk,
    const ECP_BLS381& commitment,
    const BBSSignature& sig
);

}