FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-22.04

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="3.27.2"

# Optionally install the cmake for vcpkg
COPY ./reinstall-cmake.sh /tmp/

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh

# install basic tools
RUN sudo apt-get update \
    && sudo apt-get install clang-format clang-tidy doxygen iputils-ping graphviz -y

# Options for QUIC
# ----------------
# lsquic (liblsquic in vcpkg) requires BoringSSL, which is quite large
# ngtcp2 (unofficial-ngtcp2 in vcpkg) is not built with SSL, but supports WolfSSL
# msquic requires OpenSSL, very heavy, and is quite complex to integrate
#
# RECOMMEND: custom build of ngtcp2 with WolfSSL

# Options for HTTP/HTTPS
# ----------------------
# cpp-httplib requires OpenSSL and is sync-only, but has a very easy-to-use API
# libwebsockets works with OpenSSL and mbedTLS and is async, but is much lower-level
# tlsuv works with OpenSSL and mbedTLS (with extensibility) *and* is async & easy to use, but under development and 
# uWebSockets works with OpenSSL, WolfSSL and libuv, is async, header-only and has a nice API
#
# RECOMMEND: uWebSockets with nlohmann-json; for high-performance reads, simdjson