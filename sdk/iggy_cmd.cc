#include <iostream>
#include "argh.h"

int main(int, char* argv[]) {
    argh::parser cmdl(argv);

    if (cmdl[{"-v", "--verbose"}]) {
        std::cout << "Verbose, I am.\n";
    }
    return EXIT_SUCCESS;
}
