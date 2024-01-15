#pragma once

#include "protocol.h"

namespace iggy {
namespace formats {

class BinaryFormat : public WireFormat {
public:
    BinaryFormat() = default;
};

}  // namespace formats
}  // namespace iggy
