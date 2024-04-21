#include "unit_testutils.h"
#include <fmt/format.h>
#include <iostream>
#include <reproc++/drain.hpp>
#include <reproc++/reproc.hpp>
#include <stdexcept>
#include <vector>

icp::testutil::SelfSignedCertificate::SelfSignedCertificate() {
    std::vector<std::string> arguments = {"openssl",  "req",
                                          "-x509",    "-newkey",
                                          "rsa:2048", "-nodes",
                                          "-keyout",  this->keyPath.string(),
                                          "-out",     this->certificatePath.string(),
                                          "-days",    "365",
                                          "-subj",    "/CN=localhost"};
    reproc::options options;
    options.redirect.discard = true;
    reproc::process process;

    std::string output;
    reproc::sink::string sink(output);
    auto err = process.start(arguments, options);
    if (err) {
        throw std::runtime_error(fmt::format("Failed to run OpenSSL: {}", err.message()));
    }
    err = reproc::drain(process, sink, reproc::sink::null);
    if (err) {
        throw std::runtime_error(fmt::format("Failed to read OpenSSL output: {}", err.message()));
    }
    std::cout << output << std::flush;
}

icp::testutil::SelfSignedCertificate::~SelfSignedCertificate() {
    std::filesystem::remove(this->certificatePath);
    std::filesystem::remove(this->keyPath);
}

std::filesystem::path icp::testutil::SelfSignedCertificate::generateRandomTempPath(std::string baseName) {
    return std::filesystem::temp_directory_path() / (std::to_string(std::rand()) + baseName);
}
