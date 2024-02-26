#pragma once

#include "serialization.h"

namespace iggy {
namespace serialization {

/**
 * @namespace json
 * @brief All related types for the JSON format used in Iggy's HTTP REST transport.
 */
namespace json {

/**
 * @class JsonWireFormat
 * @brief Binary serialization and deserialization for Iggy's protocol.   
 */
class JsonWireFormat : iggy::serialization::WireFormat {
public:
    JsonWireFormat() = default;
}

}  // namespace json
}  // namespace serialization
}  // namespace iggy
