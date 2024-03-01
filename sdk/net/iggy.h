#pragma once
#include <map>
#include <vector>
#include "address.h"

namespace iggy {
namespace net {

const unsigned short DEFAULT_HTTP_PORT = 3000;
const unsigned short DEFAULT_TCP_PORT = 8090;
const unsigned short DEFAULT_QUIC_PORT = 8080;

const std::string QUIC_PROTOCOL = "quic";
const std::string TCP_PROTOCOL = "tcp";
const std::string TCP_TLS_PROTOCOL = "tcp+tls";
const std::string HTTP_PROTOCOL = "http";
const std::string HTTP_TLS_PROTOCOL = "http+tls";

using iggy::net::protocol::MessageEncoding;
using iggy::net::protocol::ProtocolDefinition;

/**
 * @brief Provider that declares support and offers defaults for all Iggy C++ supported protocols.
 *
 * At this time we support iggy:quic, iggy:tcp (binary messaging) and iggy:http (with JSON messaging).
 */
class IggyProtocolProvider : public iggy::net::protocol::ProtocolProvider {
private:
    std::vector<ProtocolDefinition> supportedProtocols = {
        ProtocolDefinition(QUIC_PROTOCOL, DEFAULT_QUIC_PORT, iggy::net::transport::QUIC, true, MessageEncoding::BINARY),
        ProtocolDefinition(TCP_PROTOCOL, DEFAULT_TCP_PORT, iggy::net::transport::TCP, false, MessageEncoding::BINARY),
        ProtocolDefinition(TCP_TLS_PROTOCOL, DEFAULT_TCP_PORT, iggy::net::transport::TCP, true, MessageEncoding::BINARY),
        ProtocolDefinition(HTTP_PROTOCOL, DEFAULT_HTTP_PORT, iggy::net::transport::HTTP, false, MessageEncoding::TEXT),
        ProtocolDefinition(HTTP_TLS_PROTOCOL, DEFAULT_HTTP_PORT, iggy::net::transport::HTTP, true, MessageEncoding::TEXT)};
    std::map<std::string, ProtocolDefinition> supportedProtocolLookup;

public:
    IggyProtocolProvider();
    const std::vector<ProtocolDefinition>& getSupportedProtocols() const override;
    const ProtocolDefinition& getProtocolDefinition(const std::string& protocol) const override;
    const bool isSupported(const std::string& protocol) const override;
};

};  // namespace net
};  // namespace iggy
