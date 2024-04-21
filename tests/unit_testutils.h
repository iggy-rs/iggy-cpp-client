#pragma once

#include <catch.hpp>
#include <filesystem>
#include <string>

const char UT_TAG[] = "[Unit Tests]";

namespace icp {
namespace testutil {
/**
 * @brief A helper class for generating self-signed certificates for testing purposes using OpenSSL CLI.
 */
class SelfSignedCertificate {
private:
    std::filesystem::path certificatePath = generateRandomTempPath("cert.pem");
    std::filesystem::path keyPath = generateRandomTempPath("key.pem");

    static std::filesystem::path generateRandomTempPath(std::string baseName);

public:
    SelfSignedCertificate();
    ~SelfSignedCertificate();
    std::filesystem::path getCertificatePath() { return this->certificatePath; }
    std::filesystem::path getKeyPath() { return this->keyPath; }
};
}  // namespace testutil
}  // namespace icp
