#include "tls.h"
#include <numeric>
#include "fmt/format.h"

iggy::tls::TLSContext::TLSContext(const iggy::crypto::CertificateAuthority& certAuth,
                                  const iggy::crypto::CertificateStore& certStore,
                                  const iggy::crypto::KeyStore& keyStore,
                                  bool useClientCert,
                                  const std::vector<std::string>& ciphers)
    : certAuth(certAuth)
    , certStore(certStore)
    , keyStore(keyStore)
    , useClientCert(useClientCert)
    , ciphers(ciphers) {
    // before we make any other wolfSSL calls, make sure library is initialized once and only once
    std::call_once(sslInitDone, []() { wolfSSL_Init(); });

    // for now we only support a TLS 1.3 client context; if we generalize this code we can expand
    this->ctx = wolfSSL_CTX_new(wolfTLSv1_3_client_method());
    if (!this->ctx) {
        char* errMsg = wolfSSL_ERR_error_string(wolfSSL_ERR_get_error(), nullptr);
        throw std::runtime_error(fmt::format("Failed to allocate WolfSSL TLS context: {}", errMsg));
    }
    this->cm = wolfSSL_CTX_GetCertManager(ctx);

    // set up the supported ciphers
    std::string delimiter = ":";
    std::string joinedCiphers;

    if (!ciphers.empty()) {
        joinedCiphers = std::accumulate(std::next(ciphers.begin()), ciphers.end(), ciphers[0],
                                        [delimiter](std::string a, std::string b) { return a + delimiter + b; });
    }
    int ret = wolfSSL_CTX_set_cipher_list(this->ctx, joinedCiphers.c_str());
    if (ret != SSL_SUCCESS) {
        char* errMsg = wolfSSL_ERR_error_string(wolfSSL_ERR_get_error(), nullptr);
        throw std::runtime_error(fmt::format("Failed to set cipher list: {}", errMsg));
    }
}

iggy::tls::TLSContext::TLSContext(const TLSContext& other)
    : certAuth(other.certAuth)
    , certStore(other.certStore)
    , keyStore(other.keyStore)
    , useClientCert(useClientCert)
    , ciphers(ciphers) {
    this->ctx = wolfSSL_CTX_new(wolfTLSv1_3_client_method());
    this->cm = wolfSSL_CTX_GetCertManager(ctx);
}

iggy::tls::TLSContext::TLSContext(TLSContext&& other)
    : certAuth(certAuth)
    , certStore(certStore)
    , keyStore(keyStore)
    , useClientCert(useClientCert)
    , ciphers(ciphers) {
    this->ctx = other.ctx;
    this->cm = other.cm;
    other.ctx = nullptr;
    other.cm = nullptr;
}

iggy::tls::TLSContext::~TLSContext() {
    if (this->ctx) {
        wolfSSL_CTX_free(this->ctx);
    }
}

iggy::tls::TLSContext& iggy::tls::TLSContext::operator=(const iggy::tls::TLSContext& other) {
    if (this != &other) {
        if (this->ctx) {
            wolfSSL_CTX_free(this->ctx);
        }
        this->ctx = wolfSSL_CTX_new(wolfTLSv1_3_client_method());
    }
    return *this;
}

iggy::tls::TLSContext& iggy::tls::TLSContext::operator=(TLSContext&& other) {
    if (this != &other) {
        if (this->ctx) {
            wolfSSL_CTX_free(this->ctx);
        }
        this->ctx = other.ctx;
        other.ctx = nullptr;
    }
    return *this;
}

const std::vector<std::string> iggy::tls::TLSContext::getDefaultCipherList() {
    auto ciphers = std::vector<std::string>();
#if defined(HAVE_AESGCM) && !defined(NO_DH)
#ifdef WOLFSSL_TLS13
    ciphers.push_back("TLS13-AES128-GCM-SHA256");
#ifndef WOLFSSL_NO_TLS12
    ciphers.push_back("DHE-PSK-AES128-GCM-SHA256");
#endif
#else
    ciphers.push_back("DHE-PSK-AES128-GCM-SHA256");
#endif
#elif defined(HAVE_AESGCM) && defined(WOLFSSL_TLS13)
    ciphers.push_back("TLS13-AES128-GCM-SHA256");
#ifndef WOLFSSL_NO_TLS12
    ciphers.push_back("PSK-AES128-GCM-SHA256");
#endif
#elif defined(HAVE_NULL_CIPHER)
    ciphers.push_back("PSK-NULL-SHA256");
#elif !defined(NO_AES_CBC)
    ciphers.push_back("PSK-AES128-CBC-SHA256");
#else
    ciphers.push_back("PSK-AES128-GCM-SHA256");
#endif
    return ciphers;
}
