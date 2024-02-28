#pragma once
#include <reproc++/reproc.hpp>

/**
 * @brief Test fixture that starts and stops a Docker container for each test.
 *
 * This test fixture is meant for use in end-to-end (E2E) tests for the Iggy C++ client.
 * It will start up the latest Iggy server inside a Docker container, allow you to
 * interact with it, then stop and remove the container in the TearDown() method.
 */
class IggyRunner {
private:
    reproc::process process;

public:
    IggyRunner();
    ~IggyRunner();
};