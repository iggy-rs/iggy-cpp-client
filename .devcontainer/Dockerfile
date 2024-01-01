FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-22.04

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="3.27.2"

# set up home for vcpkg
ENV VCPKG_ROOT /usr/vcpkg

# Optionally install the cmake for vcpkg
COPY ./reinstall-cmake.sh /tmp/

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh

# install basic tools
RUN sudo apt-get update \
    && sudo apt-get install clang-format clang-tidy doxygen graphviz -y

# Install C++ dependencies using vcpkg
RUN vcpkg integrate install \
    && vcpkg install argh \
    && vcpkg install benchmark \
    && vcpkg install libuv \
    && vcpkg install lz4 \
    && vcpkg install ngtcp2 \
    && vcpkg install simdjson \
    && vcpkg install spdlog \
    && vcpkg install wolfssl
