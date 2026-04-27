#include <iostream>
#include <cstdio>
#include "crypto/bbs.hpp"
#include "crypto/amcl_utils.hpp"

namespace crypto {

void bbs_generate_keypair(BBSSecretKey& sk, BBSPublicKey& pk) {
    std::cout << "[BBS] Generate keypair started\n";

    ECP2_BLS381 g2;

    std::cout << "[BBS] Before get_g2_generator\n";
    get_g2_generator(&g2);
    std::cout << "[BBS] After get_g2_generator\n";

    std::cout << "[BBS] Before random_scalar\n";
    random_scalar(sk.x);
    std::cout << "[BBS] After random_scalar\n";

    std::cout << "[BBS] Before copy public key\n";
    ECP2_BLS381_copy(&pk.w, &g2);
    std::cout << "[BBS] After copy public key\n";

    std::cout << "[BBS] Before multiply public key\n";
    ECP2_BLS381_mul(&pk.w, sk.x);
    std::cout << "[BBS] After multiply public key\n";

    std::cout << "[BBS] Generate keypair completed\n";
}

void bbs_print_public_key(const BBSPublicKey& pk) {
    char buffer[2048];
    octet out{0, static_cast<int>(sizeof(buffer)), buffer};

    ECP2_BLS381_toOctet(&out, &pk.w);

    std::cout << "BBS public key (compressed G2): ";
    for (int i = 0; i < out.len; ++i) {
        std::printf("%02x", static_cast<unsigned char>(out.val[i]));
    }
    std::cout << "\n";
}

}