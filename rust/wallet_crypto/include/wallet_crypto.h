#pragma once

#ifdef __cplusplus
extern "C" {
#endif

unsigned int rust_random_u32(void);
unsigned char rust_sha256_first_byte(unsigned int input);

#ifdef __cplusplus
}
#endif
