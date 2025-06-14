#include <fmt/core.h>

#include "algorithm.h"
#include "cube.h"
#include "thistlethwaite_solver.h"
#include "two_phase_solver.h"

using namespace cubot;

int main(int argc, char *argv[]) {
    if (argc != 3 && argc != 7) {
        fmt::print("Usage: ./{} <thistlethwaite|twophase> <scramble> [<path1> "
                   "<path2> <path3> <path4>]\n",
                   argv[0]);
        return EXIT_FAILURE;
    }
    std::string method(argv[1]);
    Algorithm scramble(argv[2]);

    Cube cube;
    cube.Execute(scramble);

    Algorithm solution;
    if (method == "thistlethwaite") {
        ThistlethwaiteSolver thistlethwaite_solver =
            argc == 3
                ? ThistlethwaiteSolver()
                : ThistlethwaiteSolver(argv[3], argv[4], argv[5], argv[6]);
        solution = thistlethwaite_solver.Solve(cube);
    } else if (method == "twophase") {
        TwoPhaseSolver two_phase_solver =
            argc == 3 ? TwoPhaseSolver()
                      : TwoPhaseSolver(argv[3], argv[4], argv[5], argv[6]);
        solution = two_phase_solver.Solve(cube);
    } else {
        return EXIT_FAILURE;
    }

    fmt::print("{}\n", solution.ToStringNotation());

    return EXIT_SUCCESS;
}
