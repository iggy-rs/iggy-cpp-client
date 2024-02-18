#pragma once
#include <gtest/gtest.h>
#include <reproc++/reproc.hpp>

/**
 * @class DockerTest
 * @brief GoogleTest fixture that starts and stops a Docker container for each test.
 *
 * This test fixture is meant for use in end-to-end (E2E) tests for the Iggy C++ client.
 * It will start up the latest Iggy server inside a Docker container, allow you to
 * interact with it, then stop and remove the container in the TearDown() method.
 */
class DockerTest : public ::testing::Test {
protected:
    reproc::process process;

    void SetUp() override;

    void TearDown() override;
};