#include "solve.h"

#include <functional>
#include <stack>
#include <vector>

#include "algorithm.h"
#include "cube.h"

namespace cubot {

Algorithm Solve(const Cube &cube,
                std::function<int(const Cube &cube)> heuristic,
                std::function<bool(const Cube &cube)> goal_predicate,
                const std::vector<Algorithm::Turn> &turns) {
    struct Entry {
        Cube cube;
        bool goal;
        int depth;
        Algorithm::Turn turn;
    };

    std::vector<Algorithm::Turn> path;
    for (int depth = 0;; ++depth) {
        std::stack<Entry> frontier;
        frontier.push({cube, goal_predicate(cube), 0});
        path.resize(static_cast<size_t>(depth));
        while (frontier.size() > 0) {
            struct Entry entry = frontier.top();
            frontier.pop();

            if (entry.depth != 0)
                path[static_cast<size_t>(entry.depth) - 1] = entry.turn;

            if (entry.depth + heuristic(entry.cube) > depth)
                continue;

            if (entry.goal)
                return Algorithm(path);

            if (entry.depth == depth)
                continue;

            for (auto turn : turns) {
                Cube neighbor = entry.cube;
                neighbor.Execute(turn);
                bool neighbor_goal = goal_predicate(neighbor);
                int neighbor_depth = entry.depth + 1;
                frontier.push({neighbor, neighbor_goal, neighbor_depth, turn});
            }
        }
    }
}

Algorithm Solve(const Cube &cube, uint8_t *prune_table, size_t prune_table_len,
                int (*prune_state)(const Cube &cube), const Cube &goal_cube,
                const std::vector<Algorithm::Turn> &turns) {
    const int kGoalState = prune_state(goal_cube);
    auto heuristic = [&](const Cube &cube) {
        return prune_table[prune_state(cube)];
    };
    auto goal_predicate = [&](const Cube &cube) {
        return prune_state(cube) == kGoalState;
    };
    return Solve(cube, heuristic, goal_predicate, turns);
}

} // namespace cubot
