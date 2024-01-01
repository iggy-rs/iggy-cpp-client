#pragma once

#include "protocol.h"

namespace iggy {
namespace formats {

class JsonFormat : public WireFormat {
public:
    JsonFormat() = default;
};

}  // namespace formats
}  // namespace iggy