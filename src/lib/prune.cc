#include "prune.h"

#include <cstdint>
#include <deque>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "algorithm.h"
#include "cube.h"

namespace cubot {

void GeneratePruneTable(uint8_t *prune_table, size_t prune_table_len,
                        int (*calculate_state)(const Cube &cube),
                        const Cube &goal_cube,
                        const std::vector<Algorithm::Turn> &turns) {
    struct Entry {
        Cube cube;
        int state;
        uint8_t depth;
    };

    std::deque<struct Entry> frontier = {
        {goal_cube, calculate_state(goal_cube), 0}};
    std::unordered_set<int> visited = {calculate_state(goal_cube)};
    while (frontier.size() > 0) {
        struct Entry entry = frontier.front();
        frontier.pop_front();

        prune_table[entry.state] = entry.depth;

        for (auto turn : turns) {
            Cube neighbor = entry.cube;
            neighbor.Execute(turn);
            int neighbor_state = calculate_state(neighbor);
            uint8_t neighbor_depth = entry.depth + 1;
            if (visited.insert(neighbor_state).second)
                frontier.push_back({neighbor, neighbor_state, neighbor_depth});
        }
    }
}

void WritePruneTable(std::string path, uint8_t *prune_table,
                     size_t prune_table_len) {
    std::ofstream out(path, std::ios::binary);
    out.write(reinterpret_cast<const char *>(prune_table),
              static_cast<std::streamsize>(prune_table_len));
}

} // namespace cubot
