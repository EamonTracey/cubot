#ifndef SOLVE_H
#define SOLVE_H

#include <functional>
#include <vector>

#include "algorithm.h"
#include "cube.h"

namespace cubot {

Algorithm Solve(const Cube &cube,
                std::function<int(const Cube &cube)> heuristic,
                std::function<bool(const Cube &cube)> goal_predicate,
                const std::vector<Algorithm::Turn> &turns);
std::vector<Algorithm>
Solve(const Cube &cube, std::function<int(const Cube &cube)> heuristic,
      std::function<bool(const Cube &cube)> goal_predicate,
      const std::vector<Algorithm::Turn> &turns, int extra_depths,
      int max_depth);

Algorithm Solve(const Cube &cube, const uint8_t *prune_table,
                size_t prune_table_len, int (*prune_state)(const Cube &cube),
                const Cube &goal_cube,
                const std::vector<Algorithm::Turn> &turns);

} // namespace cubot

#endif
