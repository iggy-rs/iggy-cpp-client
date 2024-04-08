#include "serialization.h"
#include <fmt/format.h>
#include <utf8h/utf8.h>
#include <stdexcept>

std::string iggy::serialization::convertToUTF8(const std::string& source, bool strict) {
    if (utf8valid(source.c_str()) == 0) {
        return source;
    } else {
        if (strict) {
            throw std::invalid_argument(fmt::format("The input string is not a valid UTF-8 string: '{}'", source));
        } else {
            char* data = new char[source.size() + 1];
            std::snprintf(data, source.size() + 1, "%s", source.c_str());
            utf8makevalid(data, '?');
            return std::string(data);
        }
    }
}
