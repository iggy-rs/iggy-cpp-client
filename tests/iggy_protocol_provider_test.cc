#define CATCH_CONFIG_MAIN
#include "../sdk/net/iggy.h"
#include "unit_testutils.h"

TEST_CASE("check supported Iggy protocols", UT_TAG) {
    iggy::net::IggyProtocolProvider provider;

    SECTION("enumerate supported protocols") {
        REQUIRE(provider.getSupportedProtocols().size() == 5);
    }

    SECTION("check supported protocol definitions") {
        auto [protocolName, tlsSupported] =
            GENERATE(table<std::string, bool>({{"quic", true}, {"tcp", false}, {"tcp+tls", true}, {"http", false}, {"http+tls", true}}));

        REQUIRE(provider.isSupported(protocolName));
        REQUIRE(provider.getProtocolDefinition(protocolName).getName() == protocolName);
        REQUIRE(provider.getProtocolDefinition(protocolName).isTlsSupported() == tlsSupported);
    }

    SECTION("create addresses") {
        auto [address, protocolName, host, port] =
            GENERATE(table<std::string, std::string, std::string, int>({{"quic://localhost:1234", "quic", "localhost", 1234},
                                                                        {"tcp://localhost:1234", "tcp", "localhost", 1234},
                                                                        {"tcp+tls://localhost:1234", "tcp+tls", "localhost", 1234},
                                                                        {"http://localhost:1234", "http", "localhost", 1234},
                                                                        {"http+tls://localhost:1234", "http+tls", "localhost", 1234}}));

        auto addr = provider.createAddress(address);
        REQUIRE(addr.getProtocol() == protocolName);
        REQUIRE(addr.getHost() == host);
        REQUIRE(addr.getPort() == port);
    }
}
