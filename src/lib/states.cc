#include "states.h"

const int kPowersOfTwo[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};

int CalculateEdgeOrientationState(const Cube &cube) {
    auto &edges = cube.edges();

    int state = 0;
    for (size_t i = 0; i < 11; ++i)
        state += static_cast<int>(edges[i].orientation) * kPowersOfTwo[i];

    return state;
}
