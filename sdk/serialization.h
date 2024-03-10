#pragma once

#include <string>
#include "command.h"
#include "model.h"

namespace iggy {
namespace serialization {
class WireFormat {
private:
    std::string convertToUTF8(const std::string& source);

public:
    virtual ~WireFormat() = 0;

    template <typename T, typename std::enable_if<std::is_base_of<iggy::model::Model, T>::value>::type* = nullptr>
    T read(std::istream& in);

    template <typename T, typename std::enable_if<std::is_base_of<iggy::command::Command, T>::value>::type* = nullptr>
    void write(std::ostream& out, const T& value);
};
}  // namespace serialization
}  // namespace iggy
