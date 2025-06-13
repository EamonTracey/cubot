#include "thistlethwaite_solver.h"

#include <cstdint>

#include "algorithm.h"
#include "cube.h"
#include "prune.h"
#include "solve.h"
#include "state.h"

using namespace cubot;

static const std::vector<Algorithm::Turn> kThistlethwaitePhase1Turns = {
    Algorithm::Turn::kUpClockwise,
    Algorithm::Turn::kUpHalf,
    Algorithm::Turn::kUpCounterclockwise,
    Algorithm::Turn::kDownClockwise,
    Algorithm::Turn::kDownHalf,
    Algorithm::Turn::kDownCounterclockwise,
    Algorithm::Turn::kRightClockwise,
    Algorithm::Turn::kRightHalf,
    Algorithm::Turn::kRightCounterclockwise,
    Algorithm::Turn::kLeftClockwise,
    Algorithm::Turn::kLeftHalf,
    Algorithm::Turn::kLeftCounterclockwise,
    Algorithm::Turn::kFrontClockwise,
    Algorithm::Turn::kFrontHalf,
    Algorithm::Turn::kFrontCounterclockwise,
    Algorithm::Turn::kBackClockwise,
    Algorithm::Turn::kBackHalf,
    Algorithm::Turn::kBackCounterclockwise};

static const std::vector<Algorithm::Turn> kThistlethwaitePhase2Turns = {
    Algorithm::Turn::kUpClockwise,
    Algorithm::Turn::kUpHalf,
    Algorithm::Turn::kUpCounterclockwise,
    Algorithm::Turn::kDownClockwise,
    Algorithm::Turn::kDownHalf,
    Algorithm::Turn::kDownCounterclockwise,
    Algorithm::Turn::kRightClockwise,
    Algorithm::Turn::kRightHalf,
    Algorithm::Turn::kRightCounterclockwise,
    Algorithm::Turn::kLeftClockwise,
    Algorithm::Turn::kLeftHalf,
    Algorithm::Turn::kLeftCounterclockwise,
    Algorithm::Turn::kFrontHalf,
    Algorithm::Turn::kBackHalf};

static const std::vector<Algorithm::Turn> kThistlethwaitePhase3Turns = {
    Algorithm::Turn::kUpClockwise,
    Algorithm::Turn::kUpHalf,
    Algorithm::Turn::kUpCounterclockwise,
    Algorithm::Turn::kDownClockwise,
    Algorithm::Turn::kDownHalf,
    Algorithm::Turn::kDownCounterclockwise,
    Algorithm::Turn::kRightHalf,
    Algorithm::Turn::kLeftHalf,
    Algorithm::Turn::kFrontHalf,
    Algorithm::Turn::kBackHalf};

namespace cubot {

ThistlethwaiteSolver::ThistlethwaiteSolver() {
    GeneratePruneTable1(prune_table_1_);
    GeneratePruneTable2(prune_table_2_);
    GeneratePruneTable3(prune_table_3_);
}

Algorithm ThistlethwaiteSolver::Solve(const Cube &cube) {
    Cube copy = cube;

    Algorithm phase1 = ::Solve(copy, prune_table_1_, kThistlethwaitePhase1Size,
                               CalculateEdgeOrientationState, Cube::kSolvedCube,
                               kThistlethwaitePhase1Turns);
    copy.Execute(phase1);
    Algorithm phase2 =
        ::Solve(copy, prune_table_2_, kThistlethwaitePhase2Size,
                CalculateCornerOrientationEquatorialEdgeCombinationState,
                Cube::kSolvedCube, kThistlethwaitePhase2Turns);
    copy.Execute(phase2);
    Algorithm phase3 = ::Solve(
        copy, prune_table_3_, kThistlethwaitePhase3Size,
        CalculateThistlethwaiteMiddleEdgeCombinationFirstTetradCombinationTetradTwistState,
        Cube::kSolvedCube, kThistlethwaitePhase3Turns);
    copy.Execute(phase3);

    std::vector<Algorithm::Turn> solution;
    for (auto turn : phase1.turns())
        solution.push_back(turn);
    for (auto turn : phase2.turns())
        solution.push_back(turn);
    for (auto turn : phase3.turns())
        solution.push_back(turn);
    return solution;
}

void ThistlethwaiteSolver::GeneratePruneTable1(
    uint8_t prune_table_1[kThistlethwaitePhase1Size]) {
    GeneratePruneTable(prune_table_1_, kThistlethwaitePhase1Size,
                       CalculateEdgeOrientationState, Cube::kSolvedCube,
                       kThistlethwaitePhase1Turns);
}

void ThistlethwaiteSolver::GeneratePruneTable2(
    uint8_t prune_table_2[kThistlethwaitePhase2Size]) {
    GeneratePruneTable(prune_table_2_, kThistlethwaitePhase2Size,
                       CalculateCornerOrientationEquatorialEdgeCombinationState,
                       Cube::kSolvedCube, kThistlethwaitePhase2Turns);
}

void ThistlethwaiteSolver::GeneratePruneTable3(
    uint8_t prune_table_3[kThistlethwaitePhase3Size]) {
    GeneratePruneTable(
        prune_table_3_, kThistlethwaitePhase3Size,
        CalculateThistlethwaiteMiddleEdgeCombinationFirstTetradCombinationTetradTwistState,
        Cube::kSolvedCube, kThistlethwaitePhase3Turns);
}

} // namespace cubot
