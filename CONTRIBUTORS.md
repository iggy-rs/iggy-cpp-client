# Contributing to iggy-cpp-client

## Introduction

This is a collection of tips for developers of `iggy-cpp-client`. The initial code is being built with the latest Visual Studio Code and its C++ dev container, under `.devcontainer`. If you are using CLion, XCode or other toolchains it may not have been tested, so please report issues and we will fix them. We are also currently using [vcpkg](https://vcpkg.io) in [manifest mode](https://learn.microsoft.com/en-us/vcpkg/users/manifests) for dependency management. The appropriate packages are downloaded and compiled locally as part of the CMake build, but if you add a new dependency extra steps are required to update the manifest and CMake configuration.

## HOWTO: Test GitHub Actions locally

Our continuous integration uses GitHub Actions, and sometimes it's more convenient to first test it. The VS Code container includes the [act](https://github.com/nektos/act) extension, and you can use it to do a smoke test of the CI actions before you push to GitHub. Note that the default platform Docker image does not work, and there is no working image at this time for Ubuntu 22.04, so you should force a mapping and then run just one matrix version for 20.04 like this:

```
act -P ubuntu-20.04=catthehacker/ubuntu:full-20.04 --matrix=os:ubuntu-20.04 --no-pull
```

The Docker image required is very large, and you probably want to pull it once and then specify `--no-pull` like shown above:

```
docker pull catthehacker/ubuntu:full-20.04
```

Once it's working well locally, you can push and the CI job will automatically run the full matrix for you.

## Resources

- VS Code C++ support: https://code.visualstudio.com/docs/languages/cpp
- `vcpkg`: https://vcpkg.io
- `act` GitHub Action runner: https://github.com/nektos/act
