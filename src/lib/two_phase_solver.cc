#include "two_phase_solver.h"

#include <cstdint>
#include <string>

#include "prune.h"
#include "solve.h"
#include "state.h"

using namespace cubot;

static const std::vector<Algorithm::Turn> kTwoPhasePhase1Turns = {
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

static const std::vector<Algorithm::Turn> kTwoPhasePhase2Turns = {
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

TwoPhaseSolver::TwoPhaseSolver() {
    GeneratePruneTable1(prune_table_1_);
    GeneratePruneTable2(prune_table_2_);
    GeneratePruneTable3(prune_table_3_);
    GeneratePruneTable4(prune_table_4_);
}

TwoPhaseSolver::TwoPhaseSolver(const std::string &path1,
                               const std::string &path2,
                               const std::string &path3,
                               const std::string &path4) {
    ReadPruneTable(path1, prune_table_1_, kTwoPhasePruneTable1Size);
    ReadPruneTable(path2, prune_table_2_, kTwoPhasePruneTable2Size);
    ReadPruneTable(path3, prune_table_3_, kTwoPhasePruneTable3Size);
    ReadPruneTable(path4, prune_table_4_, kTwoPhasePruneTable4Size);
}

Algorithm TwoPhaseSolver::Solve(const Cube &cube) {
    Cube copy = cube;

    auto phase_one_heuristic = [&](const Cube &cube) {
        return std::max(
            prune_table_1_
                [CalculateEdgeOrientationEquatorialEdgeCombinationState(cube)],
            prune_table_2_
                [CalculateCornerOrientationEquatorialEdgeCombinationState(
                    cube)]);
    };
    auto phase_one_goal_predicate = [&](const Cube &cube) {
        auto &edges = cube.edges();
        auto &corners = cube.corners();
        for (size_t i = 0; i < 12; ++i)
            if (edges[i].orientation != Cube::Edge::Orientation::kCorrect)
                return false;
        for (size_t i = 0; i < 8; ++i)
            if (corners[i].orientation != Cube::Corner::Orientation::kCorrect)
                return false;
        for (size_t i = 8; i < 12; ++i)
            if (!(static_cast<int>(edges[i].solvedPosition) >= 8 ||
                  static_cast<int>(edges[i].solvedPosition) <= 11))
                return false;
        return true;
    };
    auto phase_two_heuristic = [&](const Cube &cube) {
        return std::max(
            prune_table_3_
                [CalculateG2StandingMiddleEdgePermutationTetradsCombinationState(
                    cube)],
            prune_table_4_[CalculateCornerPermutationG2SlicesCombinationState(
                cube)]);
    };
    auto phase_two_goal_predicate = [&](const Cube &cube) {
        return cube == Cube::kSolvedCube;
    };

    Algorithm phase1 = ::Solve(copy, phase_one_heuristic,
                               phase_one_goal_predicate, kTwoPhasePhase1Turns);
    copy.Execute(phase1);
    Algorithm phase2 = ::Solve(copy, phase_two_heuristic,
                               phase_two_goal_predicate, kTwoPhasePhase2Turns);

    std::vector<Algorithm::Turn> solution;
    for (auto turn : phase1.turns())
        solution.push_back(turn);
    for (auto turn : phase2.turns())
        solution.push_back(turn);
    return solution;
}

void TwoPhaseSolver::GeneratePruneTable1(
    uint8_t prune_table_1[kTwoPhasePruneTable1Size]) {
    GeneratePruneTable(prune_table_1, kTwoPhasePruneTable1Size,
                       CalculateEdgeOrientationEquatorialEdgeCombinationState,
                       Cube::kSolvedCube, kTwoPhasePhase1Turns);
}

void TwoPhaseSolver::GeneratePruneTable2(
    uint8_t prune_table_2[kTwoPhasePruneTable2Size]) {
    GeneratePruneTable(prune_table_2, kTwoPhasePruneTable2Size,
                       CalculateCornerOrientationEquatorialEdgeCombinationState,
                       Cube::kSolvedCube, kTwoPhasePhase1Turns);
}

void TwoPhaseSolver::GeneratePruneTable3(
    uint8_t prune_table_3[kTwoPhasePruneTable3Size]) {
    GeneratePruneTable(
        prune_table_3, kTwoPhasePruneTable3Size,
        CalculateG2StandingMiddleEdgePermutationTetradsCombinationState,
        Cube::kSolvedCube, kTwoPhasePhase2Turns);
}

void TwoPhaseSolver::GeneratePruneTable4(
    uint8_t prune_table_4[kTwoPhasePruneTable4Size]) {
    GeneratePruneTable(prune_table_4, kTwoPhasePruneTable4Size,
                       CalculateCornerPermutationG2SlicesCombinationState,
                       Cube::kSolvedCube, kTwoPhasePhase2Turns);
}

} // namespace cubot
