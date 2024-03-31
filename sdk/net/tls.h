#pragma once

#include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#include <wolfssl/wolfcrypt/settings.h>
#include <mutex>
#include <optional>
#include <string>
#include "crypto.h"

namespace iggy {
namespace tls {

class SSLOptions {};

/**
 * @brief An SSL/TLS context for use in secure communication.
 */
class TLSContext {
private:
    static std::once_flag sslInitDone;

    const iggy::crypto::CertificateAuthority& certAuth;
    const iggy::crypto::CertificateStore& certStore;
    const iggy::crypto::KeyStore& keyStore;

    const std::vector<std::string> ciphers;
    const bool useClientCert;

    WOLFSSL_CTX* ctx;
    WOLFSSL_CERT_MANAGER* cm;

public:
    TLSContext(const iggy::crypto::CertificateAuthority& certAuth = iggy::crypto::CertificateAuthority(),
               const iggy::crypto::CertificateStore& certStore = iggy::crypto::LocalCertificateStore(),
               const iggy::crypto::KeyStore& keyStore = iggy::crypto::LocalKeyStore(),
               bool useClientCert = false,
               const std::vector<std::string>& ciphers = getDefaultCipherList());
    TLSContext(const TLSContext& other);
    TLSContext(TLSContext&& other);
    ~TLSContext();

    TLSContext& operator=(const TLSContext& other);
    TLSContext& operator=(TLSContext&& other);

    /// @brief Gets access to the underlying C SSL context handle.
    void* getNativeHandle() const { return this->ctx; }

    /**
     * @brief Gets the default cipher list for use in SSL/TLS contexts.
     * @return A vector of cipher strings, all uppercase.
     */
    static const std::vector<std::string> getDefaultCipherList();
};
};  // namespace tls
};  // namespace iggy
