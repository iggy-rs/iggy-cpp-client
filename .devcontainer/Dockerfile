FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-22.04

# install basic tools
RUN sudo apt-get update \
    && sudo apt-get install autoconf autoconf-archive \
    clang-format clang-tidy \
    lcov doxygen graphviz -y

# Install CMake and vcpkg
COPY ./install-vcpkg.sh /tmp/

RUN chmod +x /tmp/install-vcpkg.sh && \
    /tmp/install-vcpkg.sh
