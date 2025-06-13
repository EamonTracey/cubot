#include "solve.h"

#include <stack>
#include <vector>

#include "algorithm.h"
#include "cube.h"

namespace cubot {

Algorithm Solve(const Cube &cube, uint8_t *prune_table, size_t prune_table_len,
                int (*prune_state)(const Cube &cube), const Cube &goal_cube,
                const std::vector<Algorithm::Turn> &turns) {
    struct Entry {
        Cube cube;
        int state;
        int depth;
        Algorithm::Turn turn;
    };

    const int kGoalState = prune_state(goal_cube);

    std::vector<Algorithm::Turn> path;
    for (int depth = 0;; ++depth) {
        std::stack<Entry> frontier;
        frontier.push({cube, prune_state(cube), 0});
        path.resize(static_cast<size_t>(depth));
        while (frontier.size() > 0) {
            struct Entry entry = frontier.top();
            frontier.pop();

            if (entry.depth != 0)
                path[static_cast<size_t>(entry.depth) - 1] = entry.turn;

            if (entry.depth + prune_table[entry.state] > depth)
                continue;

            if (entry.state == kGoalState)
                return Algorithm(path);

            if (entry.depth == depth)
                continue;

            for (auto turn : turns) {
                Cube neighbor = entry.cube;
                neighbor.Execute(turn);
                int neighbor_state = prune_state(neighbor);
                int neighbor_depth = entry.depth + 1;
                frontier.push({neighbor, neighbor_state, neighbor_depth, turn});
            }
        }
    }
}

} // namespace cubot
