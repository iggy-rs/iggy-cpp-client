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
