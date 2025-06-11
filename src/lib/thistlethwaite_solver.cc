#include "thistlethwaite_solver.h"

#include <cstdint>
#include <deque>
#include <stack>
#include <unordered_set>

#include "algorithm.h"
#include "cube.h"
#include "state.h"

using namespace cubot;

namespace cubot {

ThistlethwaiteSolver::ThistlethwaiteSolver() {
    GeneratePruneTable1(prune_table_1_);
}

Algorithm ThistlethwaiteSolver::Solve(const Cube &cube) {
    struct Entry {
        Cube cube;
        int state;
        int depth;
        Algorithm::Turn turn;
    };

    std::vector<Algorithm::Turn> path;

    for (int depth = 0; ; ++depth) {
        std::stack<Entry> frontier;
        frontier.push({cube, CalculateEdgeOrientationState(cube), 0});
        path.resize(static_cast<size_t>(depth));
        while (frontier.size() > 0) {
            struct Entry entry = frontier.top();
            frontier.pop();

            if (entry.depth != 0)
                path[static_cast<size_t>(entry.depth) - 1] = entry.turn;

            if (entry.depth + prune_table_1_[entry.state] > depth)
                continue;

            if (entry.state == 0)
                return Algorithm(path);

            if (entry.depth == depth)
                continue;

            for (int turn = 0; turn < 18; ++turn) {
                Cube neighbor = entry.cube;
                Algorithm::Turn neighbor_turn = static_cast<Algorithm::Turn>(turn);
                neighbor.Execute(neighbor_turn);
                int neighbor_state = CalculateEdgeOrientationState(neighbor);
                int neighbor_depth = entry.depth + 1;
                frontier.push({neighbor, neighbor_state, neighbor_depth, neighbor_turn});
            }
        }
    }
}

void ThistlethwaiteSolver::GeneratePruneTable1(uint8_t prune_table_1[2048]) {
    struct Entry {
        Cube cube;
        int state;
        uint8_t depth;
    };

    std::deque<struct Entry> frontier = {
        {Cube::kSolvedCube, CalculateEdgeOrientationState(Cube::kSolvedCube),
         0}};
    std::unordered_set<int> visited = {
        CalculateEdgeOrientationState(Cube::kSolvedCube)};
    while (frontier.size() > 0) {
        struct Entry entry = frontier.front();
        frontier.pop_front();

        prune_table_1[entry.state] = entry.depth;

        for (int turn = 0; turn < 18; ++turn) {
            Cube neighbor = entry.cube;
            neighbor.Execute(static_cast<Algorithm::Turn>(turn));
            int neighbor_state = CalculateEdgeOrientationState(neighbor);
            uint8_t neighbor_depth = entry.depth + 1;
            if (visited.insert(neighbor_state).second)
                frontier.push_back({neighbor, neighbor_state, neighbor_depth});
        }
    }
}

} // namespace cubot
