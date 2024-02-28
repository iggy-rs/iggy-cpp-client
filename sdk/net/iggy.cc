#include "iggy.h"

iggy::net::IggyProtocolProvider::IggyProtocolProvider() {
    for (const auto& protocol : this->supportedProtocols) {
        this->supportedProtocolLookup[protocol.getName()] = protocol;
    }
}

const std::vector<iggy::net::protocol::ProtocolDefinition>& iggy::net::IggyProtocolProvider::getSupportedProtocols() const {
    return this->supportedProtocols;
}

const iggy::net::protocol::ProtocolDefinition& iggy::net::IggyProtocolProvider::getProtocolDefinition(const std::string& protocol) const {
    auto normalizedName = iggy::net::protocol::normalizeProtocolName(protocol);
    auto it = this->supportedProtocolLookup.find(normalizedName);
    if (it != this->supportedProtocolLookup.end()) {
        return it->second;
    } else {
        throw std::invalid_argument("Unsupported protocol: " + protocol);
    }
}

const bool iggy::net::IggyProtocolProvider::isSupported(const std::string& protocol) const {
    return this->supportedProtocolLookup.count(iggy::net::protocol::normalizeProtocolName(protocol)) > 0;
}
