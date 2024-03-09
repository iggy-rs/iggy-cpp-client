cmake_minimum_required(VERSION 3.14)
project(iggy-cpp-client)

option(BUILD_TESTS "Build and run unit tests" OFF)
option(BUILD_DOCS "Build documentation" OFF)

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

# customize the builds of key networking components; WolfSSL is not
# well supported in vcpkg and we want to have more control here
include(ExternalProject)
execute_process(COMMAND nproc OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE NPROC)
externalproject_add(wolfssl
    GIT_REPOSITORY https://github.com/wolfSSL/wolfssl
    GIT_TAG v5.6.6-stable
    PREFIX ${CMAKE_BINARY_DIR}/wolfssl
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND autoreconf -i COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> --enable-all --enable-harden --enable-keylog-export --disable-ech
    BUILD_COMMAND make -j ${NPROC}
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

add_library(
  iggy

  sdk/client.cc
  sdk/model.cc
  sdk/serialization.cc
  sdk/net/address.cc
  sdk/net/protocol.cc
  sdk/net/iggy.cc
)
target_compile_features(iggy PRIVATE cxx_std_17)
target_include_directories(iggy PRIVATE ${SODIUM_INCLUDE_DIR} ${ADA_INCLUDE_DIR})
target_link_libraries(
  iggy PRIVATE

  ada::ada
  libuv::uv_a
  unofficial-sodium::sodium
)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()

if(BUILD_DOCS)
  add_subdirectory(docs)
endif()
