#pragma once

#include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>
#include "ada.h"

namespace iggy {
namespace crypto {

enum CertificateFormat { PEM = 1, ASN1 = 2 };

enum RevocationMethod { CRL = 1, OCSP = 2 };

/**
 * @brief A mechanism for loading public key certificates: from file; database; or a secret vault.
 */
class CertificateStore {
private:
    iggy::crypto::CertificateFormat certificateFormat = iggy::crypto::CertificateFormat::PEM;

public:
    virtual std::vector<uint8_t> getCertificate(std::string certPath) const = 0;
    virtual std::vector<uint8_t> getCertificateVerificationChain() const = 0;
};

/**
 * @brief A certificate store that loads certificates from the local filesystem.
 */
class LocalCertificateStore : CertificateStore {
private:
    std::optional<std::filesystem::path> caCertChainPath;
};

/**
 * @brief A mechanism for loading private keys: from file; database; or a secret vault.
 */
class KeyStore {
public:
    virtual std::vector<uint8_t> getPrivateKey(std::string keyPath) const = 0;
};

/**
 * @brief A key store that loads private key materials from the local filesystem.
 */
class LocalKeyStore : KeyStore {
private:
    std::optional<std::filesystem::path> privateKeyPath;
};

/**
 * @brief Object for managing PKI certificates and revocation methods.
 */
class CertificateManager {
private:
    std::string myCertificatePath;
    std::optional<std::string> trustedPeerCertificatePath;
    std::optional<std::filesystem::path> crlPath;
    std::optional<ada::url> crlUrl;
    std::optional<ada::url> ocspUrl;
};

/**
 * @brief An SSL/TLS context for use in secure communication.
 */
class SSLContext {
private:
    WOLFSSL_CTX* ctx;
    iggy::crypto::CertificateManager certManager;
    std::optional<std::vector<std::string>> ciphers;

public:
    SSLContext();
    SSLContext(const SSLContext& other);
    SSLContext(SSLContext&& other);
    ~SSLContext();

    SSLContext& operator=(const SSLContext& other);
    SSLContext& operator=(SSLContext&& other);

    /// @brief Gets access to the underlying C SSL context handle.
    void* getNativeHandle() const;
};

};  // namespace crypto
};  // namespace iggy
