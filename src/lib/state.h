#ifndef STATE_H
#define STATE_H

#include "cube.h"

namespace cubot {

/*
 * G0 = <U, D, R, L, F, B>
 * G1 = <U, D, R, L, F2, B2>
 * G2 = <U, D, R2, L2, F2, B2>
 * G3 = <U2, D2, R2, L2, F2, B2>
 * G4 = <1>
 */

// Convert a permutation of k distinct elements each in the range [0, n) to an
// integer in the range [0, nPk).
int CalculatePermutationState(const std::vector<int> &permutation, size_t n);

// Convert a combination of k distinct elements to an integer.
// TODO: Explain better.
int CalculateCombinationState(const std::vector<int> &combination);
template <typename Iterable, typename Predicate>
int CalculateCombinationState(const Iterable &elements, Predicate predicate);

int CalculateEdgePermutationState(const Cube &cube);
int CalculateCornerPermutationState(const Cube &cube);
int CalculateEdgeOrientationState(const Cube &cube);
int CalculateCornerOrientationState(const Cube &cube);
int CalculateParityState(const Cube &cube);

int CalculateEquatorialEdgeCombinationState(const Cube &cube);
int CalculateEdgeOrientationEquatorialEdgeCombinationState(const Cube &cube);
int CalculateCornerOrientationEquatorialEdgeCombinationState(const Cube &cube);

int CalculateG2SlicesCombinationTetradPairsCombinationState(const Cube &cube);
int CalculateG2StandingMiddleEdgePermutationTetradsCombinationState(
    const Cube &cube);
int CalculateG2CornerPermutationSlicesCombinationState(const Cube &cube);
int CalculateG3State(const Cube &cube);

int CalculateCrossState(const Cube &cube);

} // namespace cubot

#endif
