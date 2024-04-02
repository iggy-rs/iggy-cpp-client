cmake_minimum_required(VERSION 3.14)
project(iggy-cpp-client)

option(BUILD_TESTS "Build and run unit tests" OFF)
option(BUILD_DOCS "Build documentation" OFF)
option(ENABLE_CODE_COVERAGE "Enable coverage reporting" OFF)

# avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
  cmake_policy(SET CMP0135 NEW)
endif()

# set up library dependencies
find_package(ada CONFIG REQUIRED)
find_package(libuv CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(unofficial-sodium CONFIG REQUIRED)

# these do not correctly support CMake
find_path(ADA_INCLUDE_DIR ada.h REQUIRED)
find_path(SODIUM_INCLUDE_DIR sodium.h REQUIRED)
find_path(UTF8H_INCLUDE_DIRS "utf8h/utf8.h")

# customize the builds of key networking components; WolfSSL is not
# well supported in vcpkg and we want to have more control here
include(ExternalProject)
execute_process(COMMAND nproc OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE NPROC)
externalproject_add(wolfssl
    GIT_REPOSITORY https://github.com/wolfSSL/wolfssl
    GIT_TAG v5.7.0-stable
    PREFIX ${CMAKE_BINARY_DIR}/wolfssl
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND autoreconf -i COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> --enable-tls13 --enable-tls12 --enable-tlsx --enable-quic --enable-harden --enable-keylog-export --enable-static --enable-sys-ca-certs --disable-ech --disable-psk --disable-opensslall --disable-dtls --disable-anonymous --disable-nullcipher --disable-oldtls --disable-sslv3 --disable-webserver --disable-examples --disable-crypttests
    BUILD_COMMAND make -j ${NPROC}
    BUILD_BYPRODUCTS ${CMAKE_BINARY_DIR}/wolfssl/lib/libwolfssl.a
    INSTALL_COMMAND make install
    UPDATE_COMMAND ""
)
externalproject_add(nghttp3
    GIT_REPOSITORY https://github.com/ngtcp2/nghttp3.git
    GIT_TAG v1.2.0
    PREFIX ${CMAKE_BINARY_DIR}/nghttp3
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND git submodule update --init COMMAND autoreconf -i COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> --enable-lib-only
    BUILD_COMMAND make -j ${NPROC}
    INSTALL_COMMAND make install
    UPDATE_COMMAND ""
)
externalproject_add(ngtcp2
    GIT_REPOSITORY https://github.com/ngtcp2/ngtcp2.git
    GIT_TAG v1.3.0
    PREFIX ${CMAKE_BINARY_DIR}/ngtcp2
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND autoreconf -i COMMAND <SOURCE_DIR>/configure PKG_CONFIG_PATH=${CMAKE_BINARY_DIR}/wolfssl/lib/pkgconfig:${CMAKE_BINARY_DIR}/nghttp3/lib/pkgconfig --prefix=<INSTALL_DIR> --with-wolfssl --enable-asan --enable-lib-only
    BUILD_COMMAND make -j ${NPROC}
    INSTALL_COMMAND make install
    UPDATE_COMMAND ""
    DEPENDS nghttp3 wolfssl
)
externalproject_add(curl
    GIT_REPOSITORY https://github.com/curl/curl
    GIT_TAG curl-8_6_0
    PREFIX ${CMAKE_BINARY_DIR}/curl
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND autoreconf -i COMMAND <SOURCE_DIR>/configure PKG_CONFIG_PATH=${CMAKE_BINARY_DIR}/wolfssl/lib/pkgconfig --prefix=<INSTALL_DIR> --with-wolfssl --without-libpsl --disable-shared
    BUILD_COMMAND make -j ${NPROC}
    INSTALL_COMMAND make install
    UPDATE_COMMAND ""
    BUILD_BYPRODUCTS ${CMAKE_BINARY_DIR}/curl/lib/libcurl.a
    DEPENDS wolfssl
)

set(WOLFSSL_INCLUDE_DIR ${CMAKE_BINARY_DIR}/wolfssl/include)
set(WOLFSSL_LIB_DIR ${CMAKE_BINARY_DIR}/wolfssl/lib)
set(NGHTTP3_INCLUDE_DIR ${CMAKE_BINARY_DIR}/nghttp3/include)
set(NGTCP2_INCLUDE_DIR ${CMAKE_BINARY_DIR}/ngtcp2/include)
set(CURL_INCLUDE_DIR ${CMAKE_BINARY_DIR}/curl/include)
set(CURL_LIB_DIR ${CMAKE_BINARY_DIR}/curl/lib)

file(GLOB_RECURSE IGGY_SOURCES "sdk/*.cc")
add_library(
  iggy
  ${IGGY_SOURCES}
)
target_compile_features(iggy PRIVATE cxx_std_17)
target_include_directories(iggy PRIVATE
  ${SODIUM_INCLUDE_DIR}
  ${ADA_INCLUDE_DIR}
  ${WOLFSSL_INCLUDE_DIR}
  ${NGHTTP3_INCLUDE_DIR}
  ${NGTCP2_INCLUDE_DIR}
  ${CURL_INCLUDE_DIR}
  ${UTF8H_INCLUDE_DIRS}
)
add_dependencies(iggy curl wolfssl)
target_link_libraries(
  iggy PRIVATE

  ada::ada
  fmt::fmt
  libuv::uv_a
  unofficial-sodium::sodium
  ${CURL_LIB_DIR}/libcurl.a
  ${WOLFSSL_LIB_DIR}/libwolfssl.a
)

# even though this is related to unit tests, to get a full report we need to ensure that
# all library files are compiled with --coverage so gcno is generated properly
if(ENABLE_CODE_COVERAGE)
  include(cmake/modules/CodeCoverage.cmake)
  append_coverage_compiler_flags()
endif()

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()

if(BUILD_DOCS)
  add_subdirectory(docs)
endif()
