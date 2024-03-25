#pragma once

#include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#include <wolfssl/wolfcrypt/settings.h>
#include <optional>
#include <string>
#include "crypto.h"

namespace iggy {
namespace tls {

/**
 * @brief An SSL/TLS context for use in secure communication.
 */
class TLSContext {
private:
    const iggy::crypto::CertificateStore& certStore;
    const iggy::crypto::KeyStore& keyStore;
    std::optional<std::vector<std::string>> ciphers;

    WOLFSSL_CTX* ctx;
    iggy::crypto::CertificateAuthority certManager;

public:
    TLSContext(const iggy::crypto::CertificateStore& certStore, const iggy::crypto::KeyStore& keyStore);
    TLSContext(const TLSContext& other);
    TLSContext(TLSContext&& other);
    ~TLSContext();

    TLSContext& operator=(const TLSContext& other);
    TLSContext& operator=(TLSContext&& other);

    /// @brief Gets access to the underlying C SSL context handle.
    void* getNativeHandle() const;
};
};  // namespace tls
};  // namespace iggy
