#pragma once

#include <ada.h>
#include <vector>
#include "protocol.h"
#include "transport.h"

namespace iggy {
namespace net {
namespace address {

/***
 * @brief Logical address used in configuration and API to specify desired transport in a compact way, e.g. iggy:quic://localhost:8080.
 */
class LogicalAddress {
private:
    ada::url url;
    const iggy::net::protocol::ProtocolProvider* protocolProvider;

    const iggy::net::protocol::ProtocolDefinition& getProtocolDefinition() const;
public:
    /**
     * @brief Construct a logical address from a URL.
     * @param url URL to parse.
     * @param protocolProvider Context object providing supported protocols and default ports.
     * @throws std::invalid_argument if the URL is invalid or the protocol is unknown.
     */
    LogicalAddress(const std::string& url, const iggy::net::protocol::ProtocolProvider* protocolProvider);

    /**
     * @brief Gets the protocol; you have a guarantee that it will be one of the supported protocols from ProtocolProvider.
     */
    const std::string getProtocol() const noexcept { return url.get_protocol(); }

    /**
     * @brief Gets the hostname to connect to or raw IP address.
     */
    const std::string getHost() const noexcept { return url.get_hostname(); }

    /**
     * @brief Gets the port to connect to; protocol default port will be substituted if not specified. 
     */
    const unsigned short getPort() const;
};
}; // namespace address
}; // namespace net
}; // namespace iggy
