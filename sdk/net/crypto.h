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

/**
 * @brief Encoding format used for certificates and keys.
 */
enum CertificateFormat { PEM = 1, ASN1 = 2 };

/**
 * @brief A mechanism for loading public key certificates: from file; database;
 * or a secret vault.
 */
class CertificateStore {
private:
    CertificateFormat certificateFormat;

public:
    explicit CertificateStore(CertificateFormat format = CertificateFormat::PEM)
        : certificateFormat(format) {}

    CertificateStore(const CertificateStore& other)
        : certificateFormat(other.certificateFormat) {}

    CertificateStore(CertificateStore&& other) noexcept
        : certificateFormat(other.certificateFormat) {}

    /**
     * @brief Retrieves a certificate from the store.
     * @param certPath A slash-delimited abstract path to the certificate file;
     * this may be translated according to the backend storage to a
     * filesystem-specific path or some other convention.
     * @return A vector of bytes representing the certificate in PEM or ASN.1
     * based on the format.
     */
    virtual std::vector<uint8_t> getCertificate(std::string certPath) const = 0;

    /**
     * @brief Retrieves the certificate verification chain from root CA
     * certificate to leaf.
     * @return A vector of bytes representing the certificate chain in PEM or
     * ASN.1 based on the format.
     */
    virtual std::vector<uint8_t> getCertificateVerificationChain() const = 0;
};

/**
 * @brief A certificate store that loads certificates from the local filesystem.
 */
class LocalCertificateStore : public CertificateStore {
private:
    std::filesystem::path certDir;

public:
    explicit LocalCertificateStore(const std::filesystem::path certDir);

    /**
     * Retrieves the certificate from the filesystem, translating the abstract
     * path to a filesystem-appropriate absolute path.
     */
    std::vector<uint8_t> getCertificate(std::string certPath) const override;
};

/**
 * @brief A mechanism for loading private keys: from file; database; or a secret
 * vault.
 */
class KeyStore {
public:
    virtual std::vector<uint8_t> getPrivateKey(std::string keyPath) const = 0;
};

/**
 * @brief A key store that loads private key materials from the local
 * filesystem.
 */
class LocalKeyStore : public KeyStore {
private:
    std::optional<std::filesystem::path> privateKeyPath;
};

/**
 * @brief A mechanism for revoking certificates: through CRL or OCSP.
 */
class RevocationMethod {
public:
    RevocationMethod() = default;
    virtual ~RevocationMethod() = default;
};

/**
 * @brief Certificate revocation list (CRL)-based revocation method.
 */
class CRL : public RevocationMethod {
private:
    std::optional<std::filesystem::path> crlPath;
    std::optional<ada::url> crlUrl;

public:
    explicit CRL(std::optional<std::filesystem::path> crlPath = std::nullopt, std::optional<ada::url> crlUrl = std::nullopt)
        : crlPath(crlPath)
        , crlUrl(crlUrl) {}
};

/**
 * @brief Online Certificate Status Protocol (OCSP)-based revocation method.
 */
class OCSP : public RevocationMethod {
private:
    std::optional<ada::url> ocspOverrideUrl;

public:
    explicit OCSP(std::optional<ada::url> ocspOverrideUrl = std::nullopt)
        : ocspOverrideUrl(ocspOverrideUrl) {}
};

/**
 * @brief Authority for verifying certificates -- either through checking
 * against a centralized CA, or via a trusted peer relationship.
 */
class CertificateAuthority {
private:
    std::optional<std::string> overrideCaCertificatePath;
    std::vector<std::string> trustedPeerCertificatePaths;
    RevocationMethod& revocationMethod;

public:
    explicit CertificateAuthority(const std::optional<std::string> overrideCaCertificatePath = std::nullopt,
                                  const RevocationMethod& rand = OCSP())
        : overrideCaCertificatePath(overrideCaCertificatePath)
        , revocationMethod(revocationMethod) {}

    CertificateAuthority(const CertificateAuthority& other) = default;
    CertificateAuthority(CertificateAuthority&& other) noexcept = default;
    ~CertificateAuthority() = default;

    std::optional<std::string> getOverrideCaCertificatePath() { return this->overrideCaCertificatePath; }

    std::vector<std::string> getTrustedPeerCertificatePaths() { return this->trustedPeerCertificatePaths; }

    void addTrustedPeerCertificate(std::string certPath) { this->trustedPeerCertificatePaths.push_back(certPath); }
};

};  // namespace crypto
};  // namespace iggy
