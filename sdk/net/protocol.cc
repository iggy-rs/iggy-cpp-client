#include "address.h"
#include "protocol.h"

iggy::net::address::LogicalAddress iggy::net::protocol::ProtocolProvider::createAddress(const std::string& url) const {
    return iggy::net::address::LogicalAddress(url, this);
}

const std::string iggy::net::protocol::normalizeProtocolName(const std::string& protocol) {
    // convert to lowercase
    std::string lowerStr = protocol;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);

    // trim whitespace from the start
    auto start = lowerStr.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) {
        throw std::invalid_argument("Protocol name cannot be empty");
    }

    // trim whitespace from the end
    auto end = lowerStr.find_last_not_of(" \t\n\r\f\v");

    // return the trimmed, lowercase string
    return lowerStr.substr(start, end - start + 1);
}