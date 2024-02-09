#define FMT_HEADER_ONLY

#include <iostream>
#include "argh.h"
#include "cli/cli.h"
#include "spdlog/spdlog.h"

int main(int, char* argv[]) {
    argh::parser cmdl(argv);

    if (cmdl[iggy::cli::VERBOSE_FLAGS]) {
        spdlog::set_level(spdlog::level::debug);
    }

    if (cmdl[iggy::cli::HELP_FLAGS] || (cmdl.pos_args().size() != 2)) {
        iggy::cli::print_usage(cmdl[0]);
        return EXIT_SUCCESS;
    }

    spdlog::debug("Iggy C++ CLI");
    spdlog::debug(cmdl.pos_args()[1]);

    return EXIT_SUCCESS;
}
