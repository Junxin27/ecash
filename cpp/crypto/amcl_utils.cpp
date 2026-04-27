#include <iostream>
#include <cstring>
#include "crypto/amcl_utils.hpp"

namespace {
    csprng rng;
    bool rng_initialized = false;
}

namespace crypto {

csprng* global_rng() {
    if (!rng_initialized) {
        init_rng();
    }

    return &rng;
}

void init_rng() {
    if (rng_initialized) return;

    char raw[100];

    for (int i = 0; i < 100; ++i) {
        raw[i] = static_cast<char>((i + 1) * 13 + 7);
    }

    octet seed;
    seed.len = 100;
    seed.max = 100;
    seed.val = raw;

    CREATE_CSPRNG(&rng, &seed);
    rng_initialized = true;
}

void cleanup_rng() {
    if (!rng_initialized) return;

    KILL_CSPRNG(&rng);
    rng_initialized = false;
}

void get_g1_generator(ECP_BLS381* out) {
    ECP_BLS381_generator(out);
}

void get_g2_generator(ECP2_BLS381* out) {
    ECP2_BLS381_generator(out);
}

void random_scalar(BIG_384_58 out) {
    init_rng();

    BIG_384_58 order;
    BIG_384_58_rcopy(order, CURVE_Order_BLS381);

    BIG_384_58_randomnum(out, order, &rng);
}

void print_g1(const char* label, ECP_BLS381* p) {
    char buffer[1024];
    octet out{0, static_cast<int>(sizeof(buffer)), buffer};

    ECP_BLS381_toOctet(&out, p, true);

    std::cout << label << ": ";

    for (int i = 0; i < out.len; ++i) {
        printf("%02x", static_cast<unsigned char>(out.val[i]));
    }

    std::cout << "\n";
}

}