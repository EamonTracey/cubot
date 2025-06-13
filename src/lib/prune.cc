#include "prune.h"

#include <deque>
#include <unordered_set>
#include <vector>

#include "algorithm.h"
#include "cube.h"

#include <iostream>

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

    int actual = 0;

    std::deque<struct Entry> frontier = {
        {goal_cube, calculate_state(goal_cube), 0}};
    std::unordered_set<int> visited = {calculate_state(goal_cube)};
    while (frontier.size() > 0) {
        struct Entry entry = frontier.front();
        frontier.pop_front();

        prune_table[entry.state] = entry.depth;

        ++actual;

        for (auto turn : turns) {
            Cube neighbor = entry.cube;
            neighbor.Execute(turn);
            int neighbor_state = calculate_state(neighbor);
            uint8_t neighbor_depth = entry.depth + 1;
            if (visited.insert(neighbor_state).second)
                frontier.push_back({neighbor, neighbor_state, neighbor_depth});
        }
    }

    std::cout << "actual: " << actual << std::endl;
}

} // namespace cubot
