#include <fmt/core.h>

#include "algorithm.h"

using namespace cubot;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fmt::print("Usage: ./{} <algorithm>\n", argv[0]);
        return EXIT_FAILURE;
    }

    Algorithm algorithm(argv[1]);
    algorithm.Compress();
    fmt::print("{}\n", algorithm.ToStringNotation());

    return EXIT_SUCCESS;
}
