#ifndef STATE_H
#define STATE_H

#include "cube.h"

namespace cubot {

int CalculatePermutationState(std::vector<int> permutation, size_t n);
int CalculateCombinationState(std::vector<int> combination);

int CalculateEdgeOrientationState(const Cube &cube);
int CalculateCornerOrientationState(const Cube &cube);
int CalculateEquatorialEdgeCombinationState(const Cube &cube);

int CalculateCrossState(const Cube &cube);

} // namespace cubot

#endif
