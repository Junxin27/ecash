#pragma once

#include <cstdint>

namespace rust_bridge {
    std::uint32_t random_u32();
    std::uint8_t sha256_first_byte(std::uint32_t input);
}