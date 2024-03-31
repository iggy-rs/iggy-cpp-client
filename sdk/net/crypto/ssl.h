#pragma once

#include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#include <wolfssl/wolfcrypt/settings.h>
#include <mutex>
#include <optional>
#include <string>
#include "crypto.h"

namespace iggy {
namespace ssl {

/**
 * @brief The type of peer endpoint represented by the local end of the socket.
 */
enum PeerType { CLIENT, SERVER };

/**
 * @brief The version of the SSL/TLS protocol to use.
 *
 * We do not support the older, less-secure variations since the expectation is the library will be used in a controlled client-server
 * environment where the developer can ensure the server endpoint is adequately hardened.
 */
enum ProtocolVersion { TLSV1_2 = 0, TLSV1_3 = 1 };

/**
 * @brief Helper function to get protocol version name given the enum.
 */
std::string getProtocolVersionName(iggy::ssl::ProtocolVersion protocolVersion);

/**
 * @brief All options related to SSL/TLS are in -- what ciphers to use, client vs. server, etc..
 *
 * Mutable configuration object containing all options related to SSL/TLS. It offers reasonable defaults for a strict client. If you need to
 * talk to an SSL/TLS server that has not been hardened you may need to modify this.
 */
class SSLOptions {
private:
    PeerType peerType;
    std::optional<std::string> peerCertPath = std::nullopt;
    ProtocolVersion minimumSupportedProtocolVersion = ProtocolVersion::TLSV1_3;
    std::vector<std::string> ciphers = getDefaultCipherList(ProtocolVersion::TLSV1_3);

public:
    /**
     * Creates a default set of options for a TLS 1.3-compatible client.
     */
    explicit SSLOptions(PeerType peerType = PeerType::CLIENT)
        : peerType(peerType) {}

    /**
     * @brief Gets the default cipher list for use in SSL/TLS contexts.
     * @return A vector of cipher strings, all uppercase.
     */
    static const std::vector<std::string> getDefaultCipherList(ProtocolVersion protocolVersion);

    /**
     * @brief Gets the type of peer endpoint represented by this end of the socket.
     */
    PeerType getPeerType() const { return this->peerType; }

    /**
     * @brief Sets the type of peer endpoint represented by the local end of the socket.
     */
    void setPeerType(PeerType peerType) { this->peerType = peerType; }

    /**
     * @brief Sets the type of peer endpoint represented by the local end of the socket.
     */
    std::optional<std::string> getPeerCertificatePath() const { return this->peerCertPath; }

    /**
     * @brief Sets the path to the peer's certificate, if any, to use for verifying the peer's identity.
     */
    void setPeerCertificatePath(const std::string& peerCertPath) { this->peerCertPath = peerCertPath; }

    /**
     * @brief Gets the minimum supported protocol version for the SSL/TLS context.
     */
    ProtocolVersion getMinimumSupportedProtocolVersion() const { return this->minimumSupportedProtocolVersion; }

    /**
     * @brief Sets the minimum supported protocol version for the SSL/TLS context.
     */
    void setMinimumSupportedProtocolVersion(ProtocolVersion minimumSupportedProtocolVersion) {
        this->minimumSupportedProtocolVersion = minimumSupportedProtocolVersion;
    }

    /**
     * @brief Gets the list of requested supported ciphers; will be validated by the context during init.
     */
    const std::vector<std::string>& getCiphers() const { return this->ciphers; }

    /**
     * @brief Sets the list of requested supported ciphers; will be validated by the context during init.
     */
    void setCiphers(const std::vector<std::string>& ciphers) { this->ciphers = ciphers; }

    /**
     * @brief Sanity checks the combination of options configured by the user.
     * @param strict if true, will apply additional validations that may be more restrictive.
     *
     * Applies basic validations to the SSL options, e.g. if PeerType::SERVER is set, then a peer certificate path must be provided.
     */
    void validate(bool strict = true) const;
};

/**
 * @brief All options related to the environment library is in -- where to load CA, certificates and keys.
 *
 * Mutable configuration object containing our hooks to load CA certificates, peer & trusted certificates, and keys. It offers reasonable
 * defaults if you are loading from PEM files on the filesystem and are OK using the operating system default CA store with OCSP.
 */
class PKIEnvironment {
private:
    iggy::crypto::CertificateAuthority& certAuth;
    iggy::crypto::CertificateStore& certStore;
    iggy::crypto::KeyStore& keyStore;

public:
    PKIEnvironment(iggy::crypto::CertificateAuthority& certAuth = iggy::crypto::CertificateAuthority::getDefault(),
                   iggy::crypto::CertificateStore& certStore = iggy::crypto::LocalCertificateStore::getDefault(),
                   iggy::crypto::KeyStore& keyStore = iggy::crypto::LocalKeyStore::getDefault())
        : certAuth(certAuth)
        , certStore(certStore)
        , keyStore(keyStore) {}

    /**
     * @brief Gets the certificate authority to use for verifying peer certificates; defaults to local system CA store.
     */
    const iggy::crypto::CertificateAuthority& getCertificateAuthority() const { return this->certAuth; }

    /**
     * @brief Sets an alternative certificate authority to use for verifying peer certificates, e.g. if you use a custom CA service,
     * API-based secret store like Vault or 1Password, or a custom database.
     */
    void setCertificateAuthority(const iggy::crypto::CertificateAuthority& certAuth) { this->certAuth = certAuth; }

    /**
     * @brief Gets the certificate store to use for loading this peer's own certificate and any trusted peer certificates; defaults to a
     * local filesystem store.
     */
    const iggy::crypto::CertificateStore& getCertificateStore() const { return this->certStore; }

    /**
     * @brief Sets an alternative certificate store to use for loading this peer's own certificate and any trusted peer certificates, e.g.
     * if you use a database.
     */
    void setCertificateStore(const iggy::crypto::CertificateStore& certStore) { this->certStore = certStore; }

    /**
     * @brief Gets the certificate store to use for loading private key materials; defaults to a local filesystem store.
     */
    const iggy::crypto::KeyStore& getKeyStore() const { return this->keyStore; }

    /**
     * @brief Sets an alternative key store to use for loading private key materials, e.g. if you use an API-based secret store like Vault
     * or 1Password, cloud HSM-based vault, or a custom database.
     */
    void setKeyStore(const iggy::crypto::KeyStore& keyStore) { this->keyStore = keyStore; }
};

/**
 * @brief An SSL/TLS context for use in secure communication.
 *
 * This class is a portable wrapper around the WolfSSL context object, providing a C++ interface to the underlying C library, encapsulating
 * all the defaults of how to initialize and clean up the SSL context. It does not expose every possible option, and the intention is to
 * support OpenSSL and BoringSSL in the future.
 */
class SSLContext {
private:
    static std::once_flag sslInitDone;

    const SSLOptions& options;
    const PKIEnvironment& pkiEnv;

    WOLFSSL_CTX* ctx;
    WOLFSSL_CERT_MANAGER* cm;

public:
    explicit SSLContext(const SSLOptions& options = SSLOptions(), const PKIEnvironment& pkiEnv = PKIEnvironment());
    SSLContext(const SSLContext& other);
    SSLContext(SSLContext&& other);
    ~SSLContext();

    SSLContext& operator=(const SSLContext& other);
    SSLContext& operator=(SSLContext&& other);

    /**
     * @brief Gets access to the underlying C SSL context handle. Subsequent internal C library calls should make a static cast to ensure
     * that the expected handle type is used.
     */
    void* getNativeHandle() const { return this->ctx; }
};
};  // namespace ssl
};  // namespace iggy
