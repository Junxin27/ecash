#include <iostream>
#include <cstdio>
#include "crypto/bbs_signature.hpp"
#include "crypto/amcl_utils.hpp"

namespace crypto {

static void g1_mul(ECP_BLS381* out, const ECP_BLS381* base, const BIG_384_58 scalar) {
    ECP_BLS381_copy(out, const_cast<ECP_BLS381*>(base));
    ECP_BLS381_mul(out, const_cast<chunk*>(scalar));
}

static void g1_add(ECP_BLS381* out, const ECP_BLS381* a, const ECP_BLS381* b) {
    ECP_BLS381_copy(out, const_cast<ECP_BLS381*>(a));
    ECP_BLS381_add(out, const_cast<ECP_BLS381*>(b));
}

void bbs_sign_demo(
    const BBSSecretKey& sk,
    const BBSGenerators& gens,
    const BIG_384_58 m1,
    const BIG_384_58 m2,
    BBSSignature& sig
) {
    BIG_384_58 order, denom, inv;
    ECP_BLS381 B, t0, t1, t2;

    BIG_384_58_rcopy(order, CURVE_Order_BLS381);

    random_scalar(sig.e);
    random_scalar(sig.s);

    ECP_BLS381_copy(&B, const_cast<ECP_BLS381*>(&gens.g1));

    g1_mul(&t0, &gens.h0, sig.s);
    ECP_BLS381_add(&B, &t0);

    g1_mul(&t1, &gens.h1, m1);
    ECP_BLS381_add(&B, &t1);

    g1_mul(&t2, &gens.h2, m2);
    ECP_BLS381_add(&B, &t2);

    BIG_384_58_add(denom, sk.x, sig.e);
    BIG_384_58_norm(denom);
    BIG_384_58_mod(denom, order);
    BIG_384_58_invmodp(inv, denom, order);

    ECP_BLS381_copy(&sig.A, &B);
    ECP_BLS381_mul(&sig.A, inv);
}

bool bbs_verify_demo(
    const BBSPublicKey& pk,
    const BBSGenerators& gens,
    const BIG_384_58 m1,
    const BIG_384_58 m2,
    const BBSSignature& sig
) {
    ECP_BLS381 B, t0, t1, t2;
    ECP2_BLS381 g2, w_plus_ge;
    FP12_BLS381 left, right;

    ECP_BLS381_copy(&B, const_cast<ECP_BLS381*>(&gens.g1));

    g1_mul(&t0, &gens.h0, sig.s);
    ECP_BLS381_add(&B, &t0);

    g1_mul(&t1, &gens.h1, m1);
    ECP_BLS381_add(&B, &t1);

    g1_mul(&t2, &gens.h2, m2);
    ECP_BLS381_add(&B, &t2);

    get_g2_generator(&g2);

    ECP2_BLS381_copy(&w_plus_ge, const_cast<ECP2_BLS381*>(&pk.w));

    ECP2_BLS381 ge;
    ECP2_BLS381_copy(&ge, &g2);
    ECP2_BLS381_mul(&ge, const_cast<chunk*>(sig.e));
    ECP2_BLS381_add(&w_plus_ge, &ge);

    PAIR_BLS381_ate(&left, &w_plus_ge, const_cast<ECP_BLS381*>(&sig.A));
    PAIR_BLS381_fexp(&left);

    PAIR_BLS381_ate(&right, &g2, &B);
    PAIR_BLS381_fexp(&right);

    return FP12_BLS381_equals(&left, &right);
}

void bbs_print_signature(const BBSSignature& sig) {
    char buffer[1024];
    octet out{0, static_cast<int>(sizeof(buffer)), buffer};

    ECP_BLS381_toOctet(&out, const_cast<ECP_BLS381*>(&sig.A), true);

    std::cout << "BBS signature A: ";
    for (int i = 0; i < out.len; ++i) {
        std::printf("%02x", static_cast<unsigned char>(out.val[i]));
    }
    std::cout << "\n";
}

}