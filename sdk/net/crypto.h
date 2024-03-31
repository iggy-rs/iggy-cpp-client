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
public:
    CertificateStore() = default;
    virtual ~CertificateStore() = default;

    /**
     * @brief Retrieves a certificate from the store.
     * @param certPath A slash-delimited abstract path to the certificate file;
     * this may be translated according to the backend storage to a
     * filesystem-specific path or some other convention.
     * @return A vector of bytes representing the certificate in PEM or ASN.1.
     */
    virtual const std::vector<uint8_t> getCertificate(const std::string certPath) const = 0;
};

/**
 * @brief A certificate store that loads certificates from the local filesystem.
 */
class LocalCertificateStore : public CertificateStore {
private:
    std::optional<std::filesystem::path> certDir;

public:
    explicit LocalCertificateStore(const std::optional<std::filesystem::path> certDir = std::nullopt);
    ~LocalCertificateStore() = default;

    /**
     * Retrieves the certificate from the filesystem, translating the abstract
     * path to a filesystem-appropriate absolute path.
     */
    const std::vector<uint8_t> getCertificate(const std::string certPath) const override;
};

/**
 * @brief A mechanism for loading private keys: from file; database; or a secret
 * vault.
 */
class KeyStore {
public:
    KeyStore() = default;
    virtual ~KeyStore() = default;

    /**
     * @brief Retrieves the private key data from the store.
     * @param keyPath A slash-delimited abstract path to the key file;
     * this may be translated according to the backend storage to a
     * filesystem-specific path or some other convention.
     * @return A vector of bytes representing the key in PEM or ASN.1.
     */
    virtual const std::vector<uint8_t> getPrivateKey(const std::string keyPath) const = 0;
};

/**
 * @brief A key store that loads private key materials from the local
 * filesystem.
 */
class LocalKeyStore : public KeyStore {
private:
    std::optional<std::filesystem::path> privateKeyDir;

public:
    explicit LocalKeyStore(const std::optional<std::filesystem::path> privateKeyDir = std::nullopt);
    ~LocalKeyStore() = default;

    /**
     * Retrieves the private key materials from the filesystem, translating the abstract
     * path to a filesystem-appropriate absolute path.
     */
    const std::vector<uint8_t> getPrivateKey(const std::string keyPath) const override;
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
 * @brief Certificate revocation list (CRL)-based revocation method. If there are no CRL paths or URLs
 * specified, the CRL is assumed to be embedded in the CA certificate.
 */
class CRL : public RevocationMethod {
private:
    std::optional<std::filesystem::path> crlPath;
    std::optional<ada::url> crlUrl;

public:
    explicit CRL(std::optional<std::filesystem::path> crlPath = std::nullopt, std::optional<ada::url> crlUrl = std::nullopt)
        : crlPath(crlPath)
        , crlUrl(crlUrl) {}

    /**
     * @brief If specified (default: not), the filesystem path to the CRL file.
     */
    const std::optional<std::filesystem::path> getCrlPath() const { return this->crlPath; }

    /**
     * @brief If specified (default: not), an HTTP URL from which to load the CRL.
     */
    const std::optional<ada::url> getCrlUrl() const { return this->crlUrl; }
};

/**
 * @brief Online Certificate Status Protocol (OCSP)-based revocation method. If there is no override
 * OCSP URL specified, the OCSP URL is assumed to be embedded in the CA certificate.
 */
class OCSP : public RevocationMethod {
private:
    std::optional<ada::url> ocspOverrideUrl;

public:
    explicit OCSP(std::optional<ada::url> ocspOverrideUrl = std::nullopt)
        : ocspOverrideUrl(ocspOverrideUrl) {}

    /**
     * @brief If specified (default: not), an HTTP URL used to check the status of certificates.
     */
    const std::optional<ada::url> getOcspOverrideUrl() const { return this->ocspOverrideUrl; }
};

/**
 * @brief Authority for verifying certificates -- either through checking against a centralized CA, or via a trusted peer relationship. If
 * all defaults are taken, the system CA paths will be used, with revocation checking enabled via OCSP.
 */
class CertificateAuthority {
private:
    const std::optional<std::string> overrideCaCertificatePath;
    std::vector<std::string> trustedPeerCertificatePaths;
    const RevocationMethod& revocationMethod;

public:
    explicit CertificateAuthority(const std::optional<std::string> overrideCaCertificatePath = std::nullopt,
                                  const RevocationMethod& revocationMethod = OCSP())
        : overrideCaCertificatePath(overrideCaCertificatePath)
        , revocationMethod(revocationMethod) {}

    CertificateAuthority(const CertificateAuthority& other) = default;
    CertificateAuthority(CertificateAuthority&& other) noexcept = default;
    ~CertificateAuthority() = default;

    /**
     * @brief If specified (default: not), the filesystem path to the CA certificate path file.
     */
    const std::optional<std::string> getOverrideCaCertificatePath() const { return this->overrideCaCertificatePath; }

    /**
     * @brief Gets all specified trusted peer certificate paths, if any.
     */
    const std::vector<std::string> getTrustedPeerCertificatePaths() const { return this->trustedPeerCertificatePaths; }

    /**
     * @brief Adds a trusted peer certificate path; optional -- if none, only CA-verified certificates will be trusted.
     */
    void addTrustedPeerCertificate(const std::string certPath) { this->trustedPeerCertificatePaths.push_back(certPath); }
};

};  // namespace crypto
};  // namespace iggy
