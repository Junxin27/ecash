#include <iostream>
#include <cstdio>
#include <string>

#include "bank/bank.hpp"
#include "crypto/amcl_utils.hpp"
#include "crypto/bbs_signature.hpp"
#include "crypto/bbs_commitment.hpp"
#include "protocol/true_blind.hpp"

Bank::Bank() {
    std::cout << "[Bank] Constructor started\n";

    std::cout << "[Bank] Before bbs_generate_keypair\n";
    crypto::bbs_generate_keypair(sk_, pk_);
    std::cout << "[Bank] After bbs_generate_keypair\n";

    std::cout << "[Bank] Before bbs_init_generators\n";
    crypto::bbs_init_generators(gens_);
    std::cout << "[Bank] After bbs_init_generators\n";

    std::cout << "[Bank] Constructor completed\n";
}

void Bank::hello() const {
    std::cout << "Bank module loaded.\n";
}

const crypto::BBSPublicKey& Bank::public_key() const {
    return pk_;
}

const crypto::BBSGenerators& Bank::generators() const {
    return gens_;
}

bool Bank::validate_blind_request(const protocol::WithdrawRequest& req) const {
    if (ECP_BLS381_isinf(const_cast<ECP_BLS381*>(&req.commitment))) {
        return false;
    }

    if (ECP_BLS381_isinf(const_cast<ECP_BLS381*>(&req.blinded_commitment))) {
        return false;
    }

    if (ECP_BLS381_equals(const_cast<ECP_BLS381*>(&req.commitment),
                          const_cast<ECP_BLS381*>(&req.blinded_commitment))) {
        return false;
    }

    return true;
}

bool Bank::issue_coin(wallet::Coin& coin) const {
    BIG_384_58_copy(coin.spend_serial, coin.serial);
    BIG_384_58_copy(coin.spend_randomness, coin.randomness);
    coin.blind_issued = false;

    crypto::bbs_sign_demo(sk_, gens_, coin.spend_serial, coin.spend_randomness, coin.signature);
    return true;
}

bool Bank::issue_coin_blind(wallet::Coin& coin, const protocol::WithdrawRequest& req) const {
    if (!validate_blind_request(req)) {
        return false;
    }

    ECP_BLS381_copy(&coin.commitment, const_cast<ECP_BLS381*>(&req.commitment));
    ECP_BLS381_copy(&coin.blinded_commitment, const_cast<ECP_BLS381*>(&req.blinded_commitment));
    coin.blind_issued = true;

    crypto::bbs_sign_commitment_demo(sk_, coin.blinded_commitment, coin.signature);
    return true;
}

std::string Bank::serial_to_hex(const BIG_384_58 serial) const {
    BIG_384_58 tmp;
    BIG_384_58_copy(tmp, const_cast<chunk*>(serial));

    char raw[MODBYTES_384_58];
    BIG_384_58_toBytes(raw, tmp);

    std::string hex;
    hex.reserve(MODBYTES_384_58 * 2);

    char buf[3];
    for (int i = 0; i < MODBYTES_384_58; ++i) {
        std::snprintf(buf, sizeof(buf), "%02x", static_cast<unsigned char>(raw[i]));
        hex += buf;
    }

    return hex;
}

std::string Bank::commitment_to_hex(const ECP_BLS381& point) const {
    char buffer[1024];
    octet out{0, static_cast<int>(sizeof(buffer)), buffer};

    ECP_BLS381_toOctet(&out, const_cast<ECP_BLS381*>(&point), true);

    std::string hex;
    hex.reserve(out.len * 2);

    char buf[3];
    for (int i = 0; i < out.len; ++i) {
        std::snprintf(buf, sizeof(buf), "%02x", static_cast<unsigned char>(out.val[i]));
        hex += buf;
    }

    return hex;
}

bool Bank::deposit_coin(const wallet::Coin& coin) {
    bool valid = false;
    std::string key;

    if (coin.blind_issued) {
        valid = crypto::bbs_verify_commitment_demo(pk_, coin.blinded_commitment, coin.signature);
        key = commitment_to_hex(coin.blinded_commitment);
    } else {
        valid = crypto::bbs_verify_demo(pk_, gens_, coin.spend_serial, coin.spend_randomness, coin.signature);
        key = serial_to_hex(coin.spend_serial);
    }

    if (!valid) {
        return false;
    }

    if (spent_serials_.find(key) != spent_serials_.end()) {
        return false;
    }

    spent_serials_.insert(key);
    return true;
}

protocol::TrueBlindResponse Bank::issue_true_blind(const protocol::TrueBlindRequest& req) const {
    protocol::TrueBlindResponse resp;
    crypto::bbs_sign_commitment_demo(sk_, req.blinded_commitment, resp.blinded_signature);
    resp.issued = true;
    return resp;
}