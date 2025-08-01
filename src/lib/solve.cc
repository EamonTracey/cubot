#include "solve.h"

#include <functional>
#include <stack>
#include <vector>

#include "algorithm.h"
#include "cube.h"

static constexpr int kUp = 0;
static constexpr int kDown = 1;
static constexpr int kRight = 2;
static constexpr int kLeft = 3;
static constexpr int kFront = 4;
static constexpr int kBack = 5;
static constexpr int kLayer[] = {0, 0, 0, 1, 1, 1, 2, 2, 2,
                                 3, 3, 3, 4, 4, 4, 5, 5, 5};

namespace cubot {

Algorithm Solve(const Cube &cube,
                std::function<int(const Cube &cube)> heuristic,
                std::function<bool(const Cube &cube)> goal_predicate,
                const std::vector<Algorithm::Turn> &turns) {
    return Solve(cube, heuristic, goal_predicate, turns, -1, -1)[0];
}

std::vector<Algorithm>
Solve(const Cube &cube, std::function<int(const Cube &cube)> heuristic,
      std::function<bool(const Cube &cube)> goal_predicate,
      const std::vector<Algorithm::Turn> &turns, int extra_depths,
      int max_depth) {
    struct Entry {
        Cube cube;
        int depth;
        Algorithm::Turn turn;
    };

    std::vector<Algorithm::Turn> path;
    std::vector<Algorithm> solutions;
    for (int depth = 0;; ++depth) {
        std::stack<Entry> frontier;
        frontier.push({cube, 0, static_cast<Algorithm::Turn>(-1)});
        path.resize(static_cast<size_t>(depth));
        while (frontier.size() > 0) {
            struct Entry entry = frontier.top();
            frontier.pop();

            if (max_depth >= 0 && entry.depth > max_depth)
                return solutions;

            if (solutions.size() > 0 &&
                entry.depth > static_cast<int>(solutions[0].turns().size()) +
                                  extra_depths)
                return solutions;

            if (entry.depth != 0)
                path[static_cast<size_t>(entry.depth) - 1] = entry.turn;

            if (entry.depth + heuristic(entry.cube) > depth)
                continue;

            if (entry.depth == depth) {
                if (goal_predicate(entry.cube))
                    solutions.push_back(Algorithm(path));
                continue;
            }

            for (auto turn : turns) {
                if ((kLayer[static_cast<int>(entry.turn)] ==
                     kLayer[static_cast<int>(turn)]) ||
                    (kLayer[static_cast<int>(entry.turn)] == kDown &&
                     kLayer[static_cast<int>(turn)] == kUp) ||
                    (kLayer[static_cast<int>(entry.turn)] == kLeft &&
                     kLayer[static_cast<int>(turn)] == kRight) ||
                    (kLayer[static_cast<int>(entry.turn)] == kBack &&
                     kLayer[static_cast<int>(turn)] == kFront))
                    continue;
                Cube neighbor = entry.cube;
                neighbor.Execute(turn);
                int neighbor_depth = entry.depth + 1;
                frontier.push({neighbor, neighbor_depth, turn});
            }
        }
    }
}

Algorithm Solve(const Cube &cube, const uint8_t *prune_table,
                size_t prune_table_len, int (*prune_state)(const Cube &cube),
                const Cube &goal_cube,
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
