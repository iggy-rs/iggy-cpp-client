#include "crypto.h"
#include <fstream>
#include "fmt/format.h"

iggy::crypto::LocalCertificateStore::LocalCertificateStore(const std::filesystem::path certDir) {
    if (!std::filesystem::exists(certDir) && std::filesystem::is_directory(certDir)) {
        throw std::invalid_argument(fmt::format("certificate directory does not exist: {}", certDir.string()));
    }
    if (!std::filesystem::is_directory(certDir)) {
        throw std::invalid_argument(fmt::format("certificate directory is not a valid directory: {}", certDir.string()));
    }
    this->certDir = certDir;
}

std::vector<uint8_t> iggy::crypto::LocalCertificateStore::getCertificate(std::string certPath) const {
    std::filesystem::path certFile = this->certDir / certPath;
    if (!std::filesystem::exists(certFile)) {
        throw std::invalid_argument(fmt::format("certificate file does not exist: {}", certFile.string()));
    }
    std::ifstream certStream(certFile, std::ios::binary);
    if (!certStream.is_open()) {
        throw std::runtime_error(fmt::format("Failed to open certificate file: {}", certFile.string()));
    }
    std::vector<uint8_t> certData((std::istreambuf_iterator<char>(certStream)), std::istreambuf_iterator<char>());
    if (certData.empty()) {
        throw std::runtime_error(fmt::format("Invalid certificate file (empty): {}", certFile.string()));
    }
    return certData;
}
