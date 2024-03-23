#include "crypto.h"
#include "fmt/format.h"

iggy::crypto::SSLContext::SSLContext() {
    this->ctx = wolfSSL_CTX_new(wolfTLSv1_3_client_method());
    if (!this->ctx) {
        std::string errMsg =
            fmt::format("Failed to allocate WolfSSL TLS context: {}", wolfSSL_ERR_error_string(wolfSSL_ERR_get_error(), nullptr));
        throw std::runtime_error(errMsg);
    }
}

iggy::crypto::SSLContext::SSLContext(const SSLContext& other) {
    this->ctx = wolfSSL_CTX_new(wolfTLSv1_3_client_method());
}

iggy::crypto::SSLContext::SSLContext(SSLContext&& other) {
    this->ctx = other.ctx;
    other.ctx = nullptr;
}

iggy::crypto::SSLContext::~SSLContext() {
    if (this->ctx) {
        wolfSSL_CTX_free(this->ctx);
    }
}

iggy::crypto::SSLContext& iggy::crypto::SSLContext::operator=(const iggy::crypto::SSLContext& other) {
    if (this != &other) {
        if (this->ctx) {
            wolfSSL_CTX_free(this->ctx);
        }
        this->ctx = wolfSSL_CTX_new(wolfTLSv1_3_client_method());
    }
    return *this;
}

iggy::crypto::SSLContext& iggy::crypto::SSLContext::operator=(SSLContext&& other) {
    if (this != &other) {
        if (this->ctx) {
            wolfSSL_CTX_free(this->ctx);
        }
        this->ctx = other.ctx;
        other.ctx = nullptr;
    }
    return *this;
}

void* iggy::crypto::SSLContext::getNativeHandle() const {
    return nullptr;
}
