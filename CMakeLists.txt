cmake_minimum_required(VERSION 3.5)
project(ecash_fyp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ECASH_ENABLE_RUST "Build Rust support library" ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(BUILD_MPIN OFF CACHE BOOL "" FORCE)
set(BUILD_WCC OFF CACHE BOOL "" FORCE)
set(BUILD_X509 OFF CACHE BOOL "" FORCE)
set(BUILD_BLS OFF CACHE BOOL "" FORCE)
set(BUILD_BLS_IETF OFF CACHE BOOL "" FORCE)

set(AMCL_CURVE "BLS381" CACHE STRING "" FORCE)
set(AMCL_RSA "" CACHE STRING "" FORCE)

add_subdirectory(third_party/amcl)

set(ECASH_CORE_SOURCES
    cpp/wallet/wallet.cpp
    cpp/bank/bank.cpp
    cpp/merchant/merchant.cpp
    cpp/crypto/rust_bridge.cpp
    cpp/crypto/pairing_test.cpp
    cpp/crypto/amcl_utils.cpp
    cpp/crypto/bbs.cpp
    cpp/crypto/bbs_generators.cpp
    cpp/crypto/bbs_signature.cpp
    cpp/wallet/coin.cpp
    cpp/protocol/transaction_flow.cpp
    cpp/protocol/payment_transcript.cpp
    cpp/merchant/merchant_record.cpp
    cpp/protocol/withdraw_request.cpp
    cpp/ui/dashboard.cpp
    cpp/crypto/bbs_commitment.cpp
    cpp/protocol/true_blind.cpp
    cpp/protocol/error_demo.cpp
)

add_executable(ecash_demo
    cpp/main.cpp
    ${ECASH_CORE_SOURCES}
)

target_include_directories(ecash_demo PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

find_package(JNI REQUIRED)

add_library(ecash_jni SHARED
    cpp/jni/ecash_jni_bridge.cpp

    cpp/wallet/wallet.cpp
    cpp/wallet/coin.cpp

    cpp/bank/bank.cpp
    cpp/merchant/merchant.cpp
    cpp/merchant/merchant_record.cpp

    cpp/crypto/amcl_utils.cpp
    cpp/crypto/bbs.cpp
    cpp/crypto/bbs_generators.cpp
    cpp/crypto/bbs_signature.cpp
    cpp/crypto/bbs_commitment.cpp

    cpp/protocol/transaction_flow.cpp
    cpp/protocol/payment_transcript.cpp
    cpp/protocol/withdraw_request.cpp
    cpp/protocol/true_blind.cpp
    cpp/protocol/error_demo.cpp
)

target_include_directories(ecash_jni PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${JNI_INCLUDE_DIRS}
)

if(ECASH_ENABLE_RUST)
    add_custom_target(wallet_crypto_rust ALL
        COMMAND cargo build --release
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/rust/wallet_crypto
    )

    if(WIN32 AND MINGW)
        set(RUST_LIB_PATH ${PROJECT_SOURCE_DIR}/rust/wallet_crypto/target/release/libwallet_crypto.a)
    elseif(WIN32)
        set(RUST_LIB_PATH ${PROJECT_SOURCE_DIR}/rust/wallet_crypto/target/release/wallet_crypto.lib)
    else()
        set(RUST_LIB_PATH ${PROJECT_SOURCE_DIR}/rust/wallet_crypto/target/release/libwallet_crypto.a)
    endif()

    target_include_directories(ecash_demo PRIVATE
        ${PROJECT_SOURCE_DIR}/rust/wallet_crypto/include
    )

    target_include_directories(ecash_jni PRIVATE
        ${PROJECT_SOURCE_DIR}/rust/wallet_crypto/include
    )

    add_dependencies(ecash_demo wallet_crypto_rust)
    add_dependencies(ecash_jni wallet_crypto_rust)

    target_link_libraries(ecash_demo PRIVATE
        ${RUST_LIB_PATH}
        amcl_core
        amcl_curve_BLS381
        amcl_pairing_BLS381
    )

    target_link_libraries(ecash_jni PRIVATE
        ${RUST_LIB_PATH}
        amcl_core
        amcl_curve_BLS381
        amcl_pairing_BLS381
        ${JNI_LIBRARIES}
    )
else()
    target_link_libraries(ecash_demo PRIVATE
        amcl_core
        amcl_curve_BLS381
        amcl_pairing_BLS381
    )

    target_link_libraries(ecash_jni PRIVATE
        amcl_core
        amcl_curve_BLS381
        amcl_pairing_BLS381
        ${JNI_LIBRARIES}
    )
endif()

if(WIN32)
    target_link_libraries(ecash_demo PRIVATE
        ws2_32
        bcrypt
        userenv
        ntdll
    )

    target_link_libraries(ecash_jni PRIVATE
        ws2_32
        bcrypt
        userenv
        ntdll
    )
endif()