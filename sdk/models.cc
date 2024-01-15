#include "models.h"
#include "utils.h"

std::ostream& iggy::models::operator<<(std::ostream& os, const iggy::models::Stats& stats) {
    size_t size = stats.os_version.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(stats.os_version.c_str(), size);

    size = stats.kernel_version.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    os.write(stats.kernel_version.c_str(), size);

    return os;
}

std::istream& iggy::models::operator>>(std::istream& is, iggy::models::Stats& stats) {
    size_t size;
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    stats.os_version.resize(size);
    is.read(&stats.os_version[0], size);
    stats.os_version = iggy::utils::convertToUTF8(stats.os_version);

    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    stats.kernel_version.resize(size);
    is.read(&stats.kernel_version[0], size);
    stats.kernel_version = iggy::utils::convertToUTF8(stats.kernel_version);

    return is;
}
