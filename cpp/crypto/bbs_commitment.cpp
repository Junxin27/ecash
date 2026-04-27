#include "crypto/bbs_commitment.hpp"
#include "crypto/amcl_utils.hpp"

namespace crypto {

void bbs_sign_commitment_demo(
    const BBSSecretKey& sk,
    const ECP_BLS381& commitment,
    BBSSignature& sig
) {
    BIG_384_58 order, denom, inv;

    BIG_384_58_rcopy(order, CURVE_Order_BLS381);

    random_scalar(sig.e);
    BIG_384_58_zero(sig.s);

    BIG_384_58_add(denom, sk.x, sig.e);
    BIG_384_58_norm(denom);
    BIG_384_58_mod(denom, order);
    BIG_384_58_invmodp(inv, denom, order);

    ECP_BLS381_copy(&sig.A, const_cast<ECP_BLS381*>(&commitment));
    ECP_BLS381_mul(&sig.A, inv);
}

bool bbs_verify_commitment_demo(
    const BBSPublicKey& pk,
    const ECP_BLS381& commitment,
    const BBSSignature& sig
) {
    ECP2_BLS381 g2, w_plus_ge, ge;
    FP12_BLS381 left, right;

    get_g2_generator(&g2);

    ECP2_BLS381_copy(&w_plus_ge, const_cast<ECP2_BLS381*>(&pk.w));

    ECP2_BLS381_copy(&ge, &g2);
    ECP2_BLS381_mul(&ge, const_cast<chunk*>(sig.e));
    ECP2_BLS381_add(&w_plus_ge, &ge);

    PAIR_BLS381_ate(&left, &w_plus_ge, const_cast<ECP_BLS381*>(&sig.A));
    PAIR_BLS381_fexp(&left);

    PAIR_BLS381_ate(&right, &g2, const_cast<ECP_BLS381*>(&commitment));
    PAIR_BLS381_fexp(&right);

    return FP12_BLS381_equals(&left, &right);
}

}