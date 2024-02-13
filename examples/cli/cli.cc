#include <iostream>
#include "cli.h"

void iggy::cli::print_usage(const std::string& program_name) {
    std::cout << "Usage: " << program_name << " <command> <arg>" << std::endl;
}
