FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-22.04

# install basic tools
USER root
RUN apt-get update \
    && apt-get install autoconf autoconf-archive automake \
    clang-format clang-tidy \
    lcov doxygen graphviz -y --no-install-recommends \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Install CMake and vcpkg
COPY ./install-vcpkg.sh /tmp/
RUN chmod +x /tmp/install-vcpkg.sh && \
    /tmp/install-vcpkg.sh

# back to dev user
USER vscode
