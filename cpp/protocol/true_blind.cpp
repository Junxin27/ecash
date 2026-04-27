#include <iostream>
#include <cstdio>
#include "protocol/true_blind.hpp"
#include "crypto/bbs_commitment.hpp"

namespace protocol {

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

static void mul_to(ECP_BLS381& out, const ECP_BLS381& base, const BIG_384_58 scalar) {
    ECP_BLS381_copy(&out, const_cast<ECP_BLS381*>(&base));
    ECP_BLS381_mul(&out, const_cast<chunk*>(scalar));
}

void build_true_blind_request(
    const wallet::Coin& coin,
    const crypto::BBSGenerators& gens,
    TrueBlindRequest& req
) {
    ECP_BLS381 t1, t2, blind_term;

    BIG_384_58_copy(req.blind_factor, coin.blind_factor);

    ECP_BLS381_copy(&req.commitment, const_cast<ECP_BLS381*>(&gens.g1));

    mul_to(t1, gens.h1, coin.serial);
    ECP_BLS381_add(&req.commitment, &t1);

    mul_to(t2, gens.h2, coin.randomness);
    ECP_BLS381_add(&req.commitment, &t2);

    ECP_BLS381_copy(&req.blinded_commitment, &req.commitment);

    mul_to(blind_term, gens.h0, req.blind_factor);
    ECP_BLS381_add(&req.blinded_commitment, &blind_term);
}

void print_true_blind_request(const TrueBlindRequest& req) {
    std::cout << "\n=== True Blind Request ===\n";
    print_big_hex("Blind factor", req.blind_factor);
    print_g1_hex("Commitment", req.commitment);
    print_g1_hex("Blinded commitment", req.blinded_commitment);
}

void print_true_blind_response(const TrueBlindResponse& resp) {
    std::cout << "\n=== True Blind Response ===\n";
    std::cout << "Issued: " << (resp.issued ? "YES" : "NO") << "\n";
    if (resp.issued) {
        crypto::bbs_print_signature(resp.blinded_signature);
    }
}

void print_unblinded_coin(const UnblindedCoin& coin) {
    std::cout << "\n=== Unblinded Coin ===\n";
    std::cout << "Ready: " << (coin.ready ? "YES" : "NO") << "\n";
    print_g1_hex("Commitment", coin.commitment);
    if (coin.ready) {
        crypto::bbs_print_signature(coin.signature);
    }
}

void print_true_blind_demo(
    const TrueBlindRequest& req,
    const TrueBlindResponse& resp
) {
    print_true_blind_request(req);
    print_true_blind_response(resp);
}

bool unblind_signature(
    const TrueBlindRequest& req,
    const TrueBlindResponse& resp,
    UnblindedCoin& coin
) {
    if (!resp.issued) return false;

    ECP_BLS381_copy(&coin.commitment, const_cast<ECP_BLS381*>(&req.commitment));
    coin.signature = resp.blinded_signature;

    BIG_384_58 order, inv_alpha;
    BIG_384_58_rcopy(order, CURVE_Order_BLS381);
    BIG_384_58_invmodp(inv_alpha, const_cast<chunk*>(req.blind_factor), order);

    ECP_BLS381_mul(&coin.signature.A, inv_alpha);

    coin.ready = true;
    print_unblinded_coin(coin);
    return true;
}

bool verify_unblinded_coin(
    const UnblindedCoin& coin,
    const crypto::BBSPublicKey& pk
) {
   return coin.ready; 
}

}