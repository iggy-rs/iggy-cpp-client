#pragma once

#include <string>
#include <vector>
#include "transport.h"

namespace iggy {
namespace net {
namespace address {
class LogicalAddress;
};

namespace protocol {

/**
 * @brief Enumerates the supported message encodings.
 */
enum MessageEncoding { BINARY = 0, TEXT = 1 };

/**
 * @brief Normalizes the protocol name to a canonical form.
 */
const std::string normalizeProtocolName(const std::string& protocol);


/**
 * @brief Metadata about a protocol including its default port, transport, TLS support and message encoding.
 */
class ProtocolDefinition {
private:
    std::string name;
    unsigned short defaultPort;
    iggy::net::transport::Transport transport;
    bool tlsSupported;
    MessageEncoding messageEncoding;

public:
    ProtocolDefinition(const std::string& name,
                       unsigned short defaultPort,
                       iggy::net::transport::Transport transport,
                       bool tlsSupported,
                       MessageEncoding messageEncoding)
        : name(iggy::net::protocol::normalizeProtocolName(name))
        , defaultPort(defaultPort)
        , transport(transport)
        , tlsSupported(tlsSupported)
        , messageEncoding(messageEncoding) {}

    ProtocolDefinition() = default;
    ProtocolDefinition(const ProtocolDefinition& other) = default;

    const std::string& getName() const { return name; }
    unsigned short getDefaultPort() const { return defaultPort; }
    iggy::net::transport::Transport getTransport() const { return transport; };
    bool isTlsSupported() const { return tlsSupported; }
    MessageEncoding getMessageEncoding() const { return messageEncoding; }
};

/**
 * @brief Interface to plug in library-specific information on supported protocols.
 */
class ProtocolProvider {
public:
    ProtocolProvider() = default;
    virtual ~ProtocolProvider() = default;

    /**
     * @brief Factory method to create a logical address from a URL.
     */
    iggy::net::address::LogicalAddress createAddress(const std::string& url) const;

    /**
     * @brief Enumerates all the supported protocols in the provider.
     */
    virtual const std::vector<ProtocolDefinition>& getSupportedProtocols() const = 0;

    /**
     * @brief Given a normalized protocol name returns the definition with protocol metadata.
     */
    virtual const ProtocolDefinition& getProtocolDefinition(const std::string& protocol) const;

    /**
     * @brief Tests whether the given protocol is supported by this provider.
     */
    virtual const bool isSupported(const std::string& protocol) const;
};

};  // namespace protocol
};  // namespace net
};  // namespace iggy
