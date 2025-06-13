#ifndef PRUNE_H
#define PRUNE_H

#include <vector>

#include "algorithm.h"
#include "cube.h"

namespace cubot {

void GeneratePruneTable(uint8_t *prune_table, size_t prune_table_len,
                        int (*calculate_state)(const Cube &cube),
                        const Cube &goal_cube,
                        const std::vector<Algorithm::Turn> &turns);

}

#endif
