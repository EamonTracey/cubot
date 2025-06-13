#include <fmt/core.h>

#include "algorithm.h"
#include "cube.h"
#include "thistlethwaite_solver.h"

using namespace cubot;

int main(int argc, char *argv[]) {
    if (argc != 2 && argc != 6) {
        fmt::print("Usage: ./{} <scramble> [<path1> <path2> <path3> <path4>]\n",
                   argv[0]);
        return EXIT_FAILURE;
    }
    Algorithm scramble(argv[1]);

    // Scramble the cube.
    Cube cube;
    cube.Execute(scramble);

    // Construct the solver.
    ThistlethwaiteSolver thistlethwaite_solver =
        argc == 2 ? ThistlethwaiteSolver()
                  : ThistlethwaiteSolver(argv[2], argv[3], argv[4], argv[5]);

    // Solve.
    Algorithm solution = thistlethwaite_solver.Solve(cube);

    // Print the solution to stdout.
    fmt::print("{}\n", solution.ToStringNotation());

    return EXIT_SUCCESS;
}
