#ifndef STATE_H
#define STATE_H

#include "cube.h"

int CalculatePermutationState(std::vector<int> permutation, size_t n);

int CalculateEdgeOrientationState(const Cube &cube);
int CalculateCornerOrientationState(const Cube &cube);

int CalculateCrossState(const Cube &cube);

#endif
