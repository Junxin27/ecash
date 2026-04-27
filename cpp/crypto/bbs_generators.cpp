#include <iostream>
#include "crypto/bbs_generators.hpp"
#include "crypto/amcl_utils.hpp"

namespace crypto {

static void derive_generator(ECP_BLS381* out, const BIG_384_58 scalar) {
    ECP_BLS381 base;
    get_g1_generator(&base);
    ECP_BLS381_copy(out, &base);
    ECP_BLS381_mul(out, const_cast<chunk*>(scalar));
}

void bbs_init_generators(BBSGenerators& gens) {
    BIG_384_58 r0, r1, r2;

    get_g1_generator(&gens.g1);

    random_scalar(r0);
    random_scalar(r1);
    random_scalar(r2);

    derive_generator(&gens.h0, r0);
    derive_generator(&gens.h1, r1);
    derive_generator(&gens.h2, r2);
}

void bbs_print_generators(const BBSGenerators& gens) {
    print_g1("BBS g1", const_cast<ECP_BLS381*>(&gens.g1));
    print_g1("BBS h0", const_cast<ECP_BLS381*>(&gens.h0));
    print_g1("BBS h1", const_cast<ECP_BLS381*>(&gens.h1));
    print_g1("BBS h2", const_cast<ECP_BLS381*>(&gens.h2));
}

}