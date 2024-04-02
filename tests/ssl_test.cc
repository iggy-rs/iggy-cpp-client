#include "../sdk/net/crypto/ssl.h"
#include "unit_testutils.h"

TEST_CASE("SSL configuration", UT_TAG) {
    iggy::ssl::SSLOptions options;

    SECTION("expected basic default settings") {
        // default options should always be strictly valid
        REQUIRE_NOTHROW(options.validate(true));

        REQUIRE(options.getPeerType() == iggy::ssl::PeerType::CLIENT);
        REQUIRE(options.getPeerCertificatePath().has_value() == false);
        REQUIRE(options.getMinimumSupportedProtocolVersion() == iggy::ssl::ProtocolVersion::TLSV1_3);
    }

    SECTION("default cipher list configured") {
        auto cipherListTLSV1_2 = options.getDefaultCipherList(iggy::ssl::ProtocolVersion::TLSV1_2);
        auto cipherListTLSV1_3 = options.getDefaultCipherList(iggy::ssl::ProtocolVersion::TLSV1_3);

        REQUIRE(cipherListTLSV1_2.size() == 6);
        REQUIRE(cipherListTLSV1_3.size() == 3);

        std::string tls12Cipher = "TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384";
        std::string tls13Cipher = "TLS_CHACHA20_POLY1305_SHA256";
        CHECK(std::find(cipherListTLSV1_2.begin(), cipherListTLSV1_2.end(), tls12Cipher) != cipherListTLSV1_2.end());
        CHECK(std::find(cipherListTLSV1_3.begin(), cipherListTLSV1_3.end(), tls13Cipher) != cipherListTLSV1_3.end());
    }

    SECTION("configure bespoke cipers") {
        auto testCipher = "TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384";
        options.setCiphers({testCipher});

        auto ciphers = options.getCiphers();
        REQUIRE(ciphers.size() == 1);
        CHECK(std::find(ciphers.begin(), ciphers.end(), testCipher) != ciphers.end());

        REQUIRE_NOTHROW(options.validate(true));
    }

    SECTION("configure server options") {
        options.setPeerType(iggy::ssl::PeerType::SERVER);

        // missing certificate path
        REQUIRE_THROWS(options.validate());

        // fix the issue
        options.setPeerCertificatePath("test.pem");

        // first try strict validation, fail
        options.setMinimumSupportedProtocolVersion(iggy::ssl::ProtocolVersion::TLSV1_2);
        REQUIRE_THROWS(options.validate(true));

        // loosen the validation
        REQUIRE_NOTHROW(options.validate(false));

        // finally harden the settings and tighten up validation
        options.setMinimumSupportedProtocolVersion(iggy::ssl::ProtocolVersion::TLSV1_3);
        REQUIRE_NOTHROW(options.validate(true));
    }
}

TEST_CASE_METHOD(iggy::testutil::SelfSignedCertificate, "SSL context init", UT_TAG) {
    auto certPath = getCertificatePath().filename();
    auto keyPath = getKeyPath().filename();

    auto ca = iggy::crypto::CertificateAuthority();
    auto certStore = iggy::crypto::LocalCertificateStore(std::filesystem::temp_directory_path());
    auto keyStore = iggy::crypto::LocalKeyStore(std::filesystem::temp_directory_path());

    auto options = iggy::ssl::SSLOptions();
    options.setPeerCertificatePath(certPath);

    iggy::ssl::PKIEnvironment pkiEnv(ca, certStore, keyStore);

    SECTION("TLS version support") {
        auto [requestedVersion, minProtoVersion, maxProtoVersion] =
            GENERATE(std::make_tuple(iggy::ssl::ProtocolVersion::TLSV1_2, TLS1_2_VERSION, TLS1_3_VERSION),
                     std::make_tuple(iggy::ssl::ProtocolVersion::TLSV1_3, TLS1_3_VERSION, TLS1_3_VERSION));

        SECTION(iggy::ssl::getProtocolVersionName(requestedVersion)) {
            options.setMinimumSupportedProtocolVersion(requestedVersion);
            auto sslCtx = iggy::ssl::SSLContext(options, pkiEnv);

            REQUIRE(sslCtx.getNativeHandle() != nullptr);

            WOLFSSL_CTX* handle = static_cast<WOLFSSL_CTX*>(sslCtx.getNativeHandle());
            REQUIRE(wolfSSL_CTX_get_min_proto_version(handle) == minProtoVersion);
            REQUIRE(wolfSSL_CTX_get_max_proto_version(handle) == maxProtoVersion);

            // test copy and move constructors
            auto sslCtxNew = sslCtx;
            REQUIRE(sslCtx.getNativeHandle() != sslCtxNew.getNativeHandle());

            auto sslCtxMoved = std::move(sslCtxNew);
            REQUIRE(sslCtxNew.getNativeHandle() == nullptr);
            REQUIRE(sslCtxMoved.getNativeHandle() != nullptr);

            // test copy and move operators
            sslCtxNew = sslCtxMoved;
            REQUIRE(sslCtx.getNativeHandle() != sslCtxNew.getNativeHandle());

            iggy::ssl::SSLContext sslCtxNew2;
            sslCtxNew2 = std::move(sslCtx);
        }
    }
}

TEST_CASE("error message conversion", UT_TAG) {
    SECTION("TLS 1.2") {
        auto displayName = iggy::ssl::getProtocolVersionName(iggy::ssl::ProtocolVersion::TLSV1_2);
        REQUIRE(displayName == "TLSV1_2");
    }
    SECTION("TLS 1.3") {
        auto displayName = iggy::ssl::getProtocolVersionName(iggy::ssl::ProtocolVersion::TLSV1_3);
        REQUIRE(displayName == "TLSV1_3");
    }
}
