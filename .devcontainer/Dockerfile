FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-22.04

# install basic tools
USER root
RUN apt-get update \
    && apt-get install autoconf autoconf-archive \
    clang-format clang-tidy \
    lcov doxygen graphviz -y --no-install-recommends

# Install CMake and vcpkg
COPY ./install-vcpkg.sh /tmp/
RUN chmod +x /tmp/install-vcpkg.sh && \
    /tmp/install-vcpkg.sh

# Clean up
RUN apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# back to dev user
USER vscode
