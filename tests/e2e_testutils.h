#pragma once
#include <gtest/gtest.h>
#include <reproc++/reproc.hpp>

class DockerTest : public ::testing::Test {
protected:
    reproc::process process;

    void SetUp() override;

    void TearDown() override;
};