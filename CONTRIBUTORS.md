# Contributing to iggy-cpp-client

## Introduction

This is a collection of tips for developers of `iggy-cpp-client`. The initial code is being built with the latest Visual Studio Code and its C++ dev container, under `.devcontainer`. If you are using CLion, XCode or other toolchains it may not have been tested, so please report issues and we will fix them. We are also currently using [vcpkg](https://vcpkg.io) in [manifest mode](https://learn.microsoft.com/en-us/vcpkg/users/manifests) for dependency management. The appropriate packages are downloaded and compiled locally as part of the CMake build, but if you add a new dependency extra steps are required to update the manifest and CMake configuration.

## Tested configurations

CI runs on ubuntu-20.04 and ubuntu-22.04, and the dev containers have been tested on both x64 and arm64 (MacOS 14.x, using latest VS Code and Docker Desktop).

## HOWTO: Test GitHub Actions locally

Our continuous integration uses GitHub Actions, and sometimes it's more convenient to first test it. The VS Code container includes the [act](https://github.com/nektos/act) extension, and you can use it to do a smoke test of the CI actions before you push to GitHub:

```
act -P ubuntu-20.04=catthehacker/ubuntu:full-20.04 -P ubuntu-22.04=catthehacker/ubuntu:full-22.04
```

If you want to do a quicker test, add `--matrix=os:ubuntu-22.04` to run just a single matrix variant.

Once it's working well locally, you can push and the CI job will automatically run the full matrix for you.

## Resources

- VS Code C++ support: https://code.visualstudio.com/docs/languages/cpp
- `vcpkg`: https://vcpkg.io
- `act` GitHub Action runner: https://github.com/nektos/act
