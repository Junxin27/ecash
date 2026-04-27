#include <iostream>
#include <cstdio>
#include "wallet/coin.hpp"
#include "crypto/amcl_utils.hpp"

namespace wallet {

static void print_big_hex(const char* label, const BIG_384_58 x) {
    BIG_384_58 tmp;
    BIG_384_58_copy(tmp, const_cast<chunk*>(x));

    char raw[MODBYTES_384_58];
    BIG_384_58_toBytes(raw, tmp);

    std::cout << label << ": ";
    for (int i = 0; i < MODBYTES_384_58; ++i) {
        std::printf("%02x", static_cast<unsigned char>(raw[i]));
    }
    std::cout << "\n";
}

static void print_g1_hex(const char* label, const ECP_BLS381& p) {
    char buffer[1024];
    octet out{0, static_cast<int>(sizeof(buffer)), buffer};

    ECP_BLS381_toOctet(&out, const_cast<ECP_BLS381*>(&p), true);

    std::cout << label << ": ";
    for (int i = 0; i < out.len; ++i) {
        std::printf("%02x", static_cast<unsigned char>(out.val[i]));
    }
    std::cout << "\n";
}

void init_coin(Coin& coin) {
    crypto::random_scalar(coin.serial);
    crypto::random_scalar(coin.randomness);
    crypto::random_scalar(coin.value);
    crypto::random_scalar(coin.blind_factor);

    ECP_BLS381_inf(&coin.commitment);
    ECP_BLS381_inf(&coin.blinded_commitment);

    BIG_384_58_copy(coin.spend_serial, coin.serial);
    BIG_384_58_copy(coin.spend_randomness, coin.randomness);

    coin.spent = false;
    coin.blind_issued = false;
}

void print_coin(const Coin& coin) {
    std::cout << "Coin status: " << (coin.spent ? "SPENT" : "UNSPENT") << "\n";
    std::cout << "Issue mode: " << (coin.blind_issued ? "BLIND" : "NORMAL") << "\n";

    print_big_hex("Coin serial", coin.serial);
    print_big_hex("Coin randomness", coin.randomness);
    print_big_hex("Coin value", coin.value);
    print_big_hex("Blind factor", coin.blind_factor);

    print_g1_hex("Commitment", coin.commitment);
    print_g1_hex("Blinded commitment", coin.blinded_commitment);

    print_big_hex("Spend serial", coin.spend_serial);
    print_big_hex("Spend randomness", coin.spend_randomness);

    crypto::bbs_print_signature(coin.signature);
}

}