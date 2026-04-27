#include <iostream>
#include "crypto/pairing_test.hpp"

extern "C" {
#include "pair_BLS381.h"
#include "ecp_BLS381.h"
#include "ecp2_BLS381.h"
#include "fp12_BLS381.h"
#include "randapi.h"
}

namespace crypto {

void pairing_smoke_test() {
    ECP_BLS381 g1;
    ECP2_BLS381 g2;
    FP12_BLS381 gt;

    ECP_BLS381_generator(&g1);
    ECP2_BLS381_generator(&g2);

    PAIR_BLS381_ate(&gt, &g2, &g1);
    PAIR_BLS381_fexp(&gt);

    if (FP12_BLS381_isunity(&gt)) {
        std::cout << "Pairing test failed: result is identity.\n";
    } else {
        std::cout << "Pairing test passed: non-trivial pairing computed.\n";
    }
}

}