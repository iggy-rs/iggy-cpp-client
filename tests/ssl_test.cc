#include "../sdk/net/crypto/ssl.h"
#include "unit_testutils.h"

TEST_CASE("SSL configuration", UT_TAG) {
    iggy::ssl::SSLOptions options;
    auto cipherListTLSV1_2 = options.getDefaultCipherList(iggy::ssl::ProtocolVersion::TLSV1_2);
    auto cipherListTLSV1_3 = options.getDefaultCipherList(iggy::ssl::ProtocolVersion::TLSV1_3);

    REQUIRE(cipherListTLSV1_2.size() == 6);
    REQUIRE(cipherListTLSV1_3.size() == 3);

    std::string tls12Cipher = "TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384";
    std::string tls13Cipher = "TLS_CHACHA20_POLY1305_SHA256";
    CHECK(std::find(cipherListTLSV1_2.begin(), cipherListTLSV1_2.end(), tls12Cipher) != cipherListTLSV1_2.end());
    CHECK(std::find(cipherListTLSV1_3.begin(), cipherListTLSV1_3.end(), tls13Cipher) != cipherListTLSV1_3.end());
}
