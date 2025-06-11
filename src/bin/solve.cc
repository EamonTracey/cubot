#include <fmt/core.h>

#include "algorithm.h"
#include "cube.h"
#include "thistlethwaite_solver.h"

using namespace cubot;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fmt::print("Usage: ./{} <scramble>\n", argv[0]);
        return EXIT_FAILURE;
    }
    Algorithm scramble(argv[1]);

    // Scramble the cube.
    Cube cube;
    cube.Execute(scramble);

    // Construct the solver.
    ThistlethwaiteSolver thistlethwaite_solver;

    // Solve.
    Algorithm solution = thistlethwaite_solver.Solve(cube);

    // Print the solution to stdout.
    fmt::print("{}\n", solution.ToStringNotation());

    return EXIT_SUCCESS;
}
