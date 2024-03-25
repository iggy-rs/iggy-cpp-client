#include "tls.h"
#include "fmt/format.h"

iggy::tls::TLSContext::TLSContext(const iggy::crypto::CertificateStore& certStore, const iggy::crypto::KeyStore& keyStore)
    : certStore(certStore)
    , keyStore(keyStore) {
    this->ctx = wolfSSL_CTX_new(wolfTLSv1_3_client_method());
    if (!this->ctx) {
        std::string errMsg =
            fmt::format("Failed to allocate WolfSSL TLS context: {}", wolfSSL_ERR_error_string(wolfSSL_ERR_get_error(), nullptr));
        throw std::runtime_error(errMsg);
    }
}

iggy::tls::TLSContext::TLSContext(const TLSContext& other)
    : certStore(other.certStore)
    , keyStore(other.keyStore) {
    this->ctx = wolfSSL_CTX_new(wolfTLSv1_3_client_method());
}

iggy::tls::TLSContext::TLSContext(TLSContext&& other)
    : certStore(other.certStore)
    , keyStore(other.keyStore) {
    this->ctx = other.ctx;
    other.ctx = nullptr;
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

void* iggy::tls::TLSContext::getNativeHandle() const {
    return this->ctx;
}
