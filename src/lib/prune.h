#ifndef PRUNE_H
#define PRUNE_H

#include <cstdint>
#include <string>
#include <vector>

#include "algorithm.h"
#include "cube.h"

namespace cubot {

void GeneratePruneTable(uint8_t *prune_table, size_t prune_table_len,
                        int (*calculate_state)(const Cube &cube),
                        const Cube &goal_cube,
                        const std::vector<Algorithm::Turn> &turns);

void WritePruneTable(const std::string &path, const uint8_t *prune_table,
                     size_t prune_table_len);

void ReadPruneTable(const std::string &path, uint8_t *prune_table,
                    size_t prune_table_len);

} // namespace cubot

#endif
