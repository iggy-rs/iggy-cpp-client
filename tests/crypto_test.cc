#include "../sdk/net/crypto/crypto.h"
#include "unit_testutils.h"

TEST_CASE_METHOD(iggy::testutil::SelfSignedCertificate, "certificate loading", UT_TAG) {
    auto tmpDir = std::filesystem::temp_directory_path();
    iggy::crypto::CertificateStore* cs = new iggy::crypto::LocalCertificateStore(tmpDir);
    auto cert = cs->getCertificate(getCertificatePath().filename());
    REQUIRE(cert.size() > 0);
    delete cs;
}

TEST_CASE_METHOD(iggy::testutil::SelfSignedCertificate, "private key loading", UT_TAG) {
    auto tmpDir = std::filesystem::temp_directory_path();
    iggy::crypto::KeyStore* ks = new iggy::crypto::LocalKeyStore(tmpDir);
    auto pk = ks->getPrivateKey(getKeyPath().filename());
    REQUIRE(pk.size() > 0);
    delete ks;
}

TEST_CASE("revocation methods", UT_TAG) {
    SECTION("default CRL configuration") {
        iggy::crypto::CRL crl;

        REQUIRE(!crl.getCrlPath().has_value());
        REQUIRE(!crl.getCrlUrl().has_value());
    }

    SECTION("default OCSP configuration") {
        iggy::crypto::OCSP ocsp;

        REQUIRE(!ocsp.getOcspOverrideUrl().has_value());
        REQUIRE(ocsp.isStaplingEnabled());
    }
}
