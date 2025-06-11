#include <cstdlib>
#include <deque>
#include <unordered_set>

#include <fmt/core.h>

#include "algorithm.h"
#include "cube.h"
#include "state.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fmt::print("Usage: ./{} <scramble>\n", argv[0]);
        return EXIT_FAILURE;
    }
    Algorithm scramble(argv[1]);

    // Initialize the solved cube and calculate the goal state.
    Cube cube;
    const int kGoalState = CalculateCrossState(cube);

    // Scramble the cube.
    cube.Execute(scramble);
    int state = CalculateCrossState(cube);

    // Perform a brute-force breadth first search to solve the cross.
    Algorithm solution;
    struct Entry {
        Cube cube;
        int state;
        Algorithm path;
    };
    std::deque<struct Entry> frontier = {
        {cube, state, Algorithm(std::vector<Algorithm::Turn>{})}};
    std::unordered_set<int> visited = {state};
    while (frontier.size() > 0) {
        struct Entry entry = frontier.front();
        frontier.pop_front();

        if (entry.state == kGoalState) {
            solution = entry.path;
            break;
        }

        for (int turn = 0; turn < 18; ++turn) {
            Cube neighbor = entry.cube;
            neighbor.Execute(static_cast<Algorithm::Turn>(turn));
            int neighbor_state = CalculateCrossState(neighbor);
            if (visited.insert(neighbor_state).second) {
                auto neighbor_turns = entry.path.turns();
                neighbor_turns.push_back(static_cast<Algorithm::Turn>(turn));
                Algorithm neighbor_path = Algorithm(neighbor_turns);
                frontier.push_back({neighbor, neighbor_state, neighbor_path});
            }
        }
    }

    // Print the solution to stdout.
    fmt::print("{}\n", solution.ToStringNotation());

    return EXIT_SUCCESS;
}
