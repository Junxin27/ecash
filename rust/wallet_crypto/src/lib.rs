use rand::Rng;
use sha2::{Digest, Sha256};

#[no_mangle]
pub extern "C" fn rust_random_u32() -> u32 {
    let mut rng = rand::thread_rng();
    rng.gen::<u32>()
}

#[no_mangle]
pub extern "C" fn rust_sha256_first_byte(input: u32) -> u8 {
    let bytes = input.to_le_bytes();
    let hash = Sha256::digest(bytes);
    hash[0]
}