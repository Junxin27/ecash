#pragma once

extern "C" {
#include "pair_BLS381.h"
#include "ecp_BLS381.h"
#include "ecp2_BLS381.h"
#include "fp12_BLS381.h"
#include "randapi.h"
}

namespace crypto {

csprng* global_rng();

void init_rng();
void cleanup_rng();

void get_g1_generator(ECP_BLS381* out);
void get_g2_generator(ECP2_BLS381* out);

void random_scalar(BIG_384_58 out);
void print_g1(const char* label, ECP_BLS381* p);

}