#ifndef SOLVE_H
#define SOLVE_H

#include "algorithm.h"
#include "cube.h"

namespace cubot {

Algorithm Solve(const Cube &cube, uint8_t *prune_table, size_t prune_table_len,
                int (*calculate_state)(const Cube &cube), const Cube &goal_cube,
                const std::vector<Algorithm::Turn> &turns);

} // namespace cubot

#endif
