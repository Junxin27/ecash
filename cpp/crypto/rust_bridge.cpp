#include "crypto/rust_bridge.hpp"
#include "wallet_crypto.h"

namespace rust_bridge {

std::uint32_t random_u32() {
    return rust_random_u32();
}

std::uint8_t sha256_first_byte(std::uint32_t input) {
    return rust_sha256_first_byte(input);
}

}