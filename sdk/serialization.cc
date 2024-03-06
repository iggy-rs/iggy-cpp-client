#include "serialization.h"
#include <unicode/ucnv.h>
#include <stdexcept>

std::string iggy::serialization::WireFormat::convertToUTF8(const std::string& source) {
    UErrorCode status = U_ZERO_ERROR;
    char target[256];

    // Convert the string to UTF-8
    ucnv_convert("UTF-8", "UTF-16", target, sizeof(target), source.c_str(), source.length(), &status);
    if (U_FAILURE(status)) {
        throw std::runtime_error("Failed to convert string to UTF-8");
    }

    return std::string(target);
}