#pragma once

extern "C" {
#include "ecp_BLS381.h"
}

namespace crypto {

struct BBSGenerators {
    ECP_BLS381 g1;
    ECP_BLS381 h0;
    ECP_BLS381 h1;
    ECP_BLS381 h2;
};

void bbs_init_generators(BBSGenerators& gens);
void bbs_print_generators(const BBSGenerators& gens);

}