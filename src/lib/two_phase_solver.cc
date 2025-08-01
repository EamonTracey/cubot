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
    prune_table_1_ = static_cast<uint8_t *>(
        malloc(kTwoPhasePruneTable1Size * sizeof(uint8_t)));
    prune_table_2_ = static_cast<uint8_t *>(
        malloc(kTwoPhasePruneTable2Size * sizeof(uint8_t)));
    prune_table_3_ = static_cast<uint8_t *>(
        malloc(kTwoPhasePruneTable3Size * sizeof(uint8_t)));
    prune_table_4_ = static_cast<uint8_t *>(
        malloc(kTwoPhasePruneTable4Size * sizeof(uint8_t)));
    GeneratePruneTable1(prune_table_1_);
    GeneratePruneTable2(prune_table_2_);
    GeneratePruneTable3(prune_table_3_);
    GeneratePruneTable4(prune_table_4_);
}

TwoPhaseSolver::TwoPhaseSolver(const std::string &path1,
                               const std::string &path2,
                               const std::string &path3,
                               const std::string &path4) {
    prune_table_1_ = static_cast<uint8_t *>(
        malloc(kTwoPhasePruneTable1Size * sizeof(uint8_t)));
    prune_table_2_ = static_cast<uint8_t *>(
        malloc(kTwoPhasePruneTable2Size * sizeof(uint8_t)));
    prune_table_3_ = static_cast<uint8_t *>(
        malloc(kTwoPhasePruneTable3Size * sizeof(uint8_t)));
    prune_table_4_ = static_cast<uint8_t *>(
        malloc(kTwoPhasePruneTable4Size * sizeof(uint8_t)));
    ReadPruneTable(path1, prune_table_1_, kTwoPhasePruneTable1Size);
    ReadPruneTable(path2, prune_table_2_, kTwoPhasePruneTable2Size);
    ReadPruneTable(path3, prune_table_3_, kTwoPhasePruneTable3Size);
    ReadPruneTable(path4, prune_table_4_, kTwoPhasePruneTable4Size);
}

Algorithm TwoPhaseSolver::Solve(const Cube &cube, int extra_depths) const {
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
            if (!(static_cast<int>(edges[i].solved_position) >= 8 ||
                  static_cast<int>(edges[i].solved_position) <= 11))
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

    Algorithm phase1;
    Algorithm phase2;
    std::vector<Algorithm> potential_phase1s =
        ::Solve(cube, phase_one_heuristic, phase_one_goal_predicate,
                kTwoPhasePhase1Turns, extra_depths, -1);
    for (auto &potential_phase1 : potential_phase1s) {
        Cube copy = cube;
        copy.Execute(potential_phase1);

        int max_depth = -1;
        if (phase2.turns().size() > 0) {
            max_depth =
                static_cast<int>(phase1.turns().size() + phase2.turns().size() -
                                 potential_phase1.turns().size() - 1);
        }

        std::vector<Algorithm> potential_phase2s =
            ::Solve(copy, phase_two_heuristic, phase_two_goal_predicate,
                    kTwoPhasePhase2Turns, -1, max_depth);
        if (potential_phase2s.size() == 0)
            continue;
        Algorithm potential_phase2 = potential_phase2s[0];

        phase1 = potential_phase1;
        phase2 = potential_phase2;
    }

    std::vector<Algorithm::Turn> phases;
    for (auto turn : phase1.turns())
        phases.push_back(turn);
    for (auto turn : phase2.turns())
        phases.push_back(turn);

    Algorithm solution(phases);
    solution.Compress();
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
