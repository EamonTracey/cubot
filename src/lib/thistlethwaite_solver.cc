#include "thistlethwaite_solver.h"

#include <cstdint>
#include <fstream>
#include <vector>

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

static const std::vector<Algorithm::Turn> kThistlethwaitePhase4Turns = {
    Algorithm::Turn::kUpHalf,    Algorithm::Turn::kDownHalf,
    Algorithm::Turn::kRightHalf, Algorithm::Turn::kLeftHalf,
    Algorithm::Turn::kFrontHalf, Algorithm::Turn::kBackHalf};

namespace cubot {

ThistlethwaiteSolver::ThistlethwaiteSolver() {
    GeneratePruneTable1(prune_table_1_);
    GeneratePruneTable2(prune_table_2_);
    GeneratePruneTable3(prune_table_3_);
    GeneratePruneTable4(prune_table_4_);
}

ThistlethwaiteSolver::ThistlethwaiteSolver(const std::string &path1,
                                           const std::string &path2,
                                           const std::string &path3,
                                           const std::string &path4) {
    std::ifstream in1(path1, std::ios::binary);
    in1.read(reinterpret_cast<char *>(prune_table_1_),
             static_cast<std::streamsize>(kThistlethwaitePhase1Size));
    std::ifstream in2(path2, std::ios::binary);
    in2.read(reinterpret_cast<char *>(prune_table_2_),
             static_cast<std::streamsize>(kThistlethwaitePhase2Size));
    std::ifstream in3(path3, std::ios::binary);
    in3.read(reinterpret_cast<char *>(prune_table_3_),
             static_cast<std::streamsize>(kThistlethwaitePhase3Size));
    std::ifstream in4(path4, std::ios::binary);
    in4.read(reinterpret_cast<char *>(prune_table_4_),
             static_cast<std::streamsize>(kThistlethwaitePhase4Size));
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
    Algorithm phase3 =
        ::Solve(copy, prune_table_3_, kThistlethwaitePhase3Size,
                CalculateG2MiddleEdgeCombinationTetradPairsCombinationState,
                Cube::kSolvedCube, kThistlethwaitePhase3Turns);
    copy.Execute(phase3);
    Algorithm phase4 = ::Solve(copy, prune_table_4_, kThistlethwaitePhase4Size,
                               CalculateG3State, Cube::kSolvedCube,
                               kThistlethwaitePhase4Turns);

    std::vector<Algorithm::Turn> solution;
    for (auto turn : phase1.turns())
        solution.push_back(turn);
    for (auto turn : phase2.turns())
        solution.push_back(turn);
    for (auto turn : phase3.turns())
        solution.push_back(turn);
    for (auto turn : phase4.turns())
        solution.push_back(turn);
    return solution;
}

void ThistlethwaiteSolver::GeneratePruneTable1(
    uint8_t prune_table_1[kThistlethwaitePhase1Size]) {
    GeneratePruneTable(prune_table_1, kThistlethwaitePhase1Size,
                       CalculateEdgeOrientationState, Cube::kSolvedCube,
                       kThistlethwaitePhase1Turns);
}

void ThistlethwaiteSolver::GeneratePruneTable2(
    uint8_t prune_table_2[kThistlethwaitePhase2Size]) {
    GeneratePruneTable(prune_table_2, kThistlethwaitePhase2Size,
                       CalculateCornerOrientationEquatorialEdgeCombinationState,
                       Cube::kSolvedCube, kThistlethwaitePhase2Turns);
}

void ThistlethwaiteSolver::GeneratePruneTable3(
    uint8_t prune_table_3[kThistlethwaitePhase3Size]) {
    GeneratePruneTable(
        prune_table_3, kThistlethwaitePhase3Size,
        CalculateG2MiddleEdgeCombinationTetradPairsCombinationState,
        Cube::kSolvedCube, kThistlethwaitePhase3Turns);
}

void ThistlethwaiteSolver::GeneratePruneTable4(
    uint8_t prune_table_4[kThistlethwaitePhase4Size]) {
    GeneratePruneTable(prune_table_4, kThistlethwaitePhase4Size,
                       CalculateG3State, Cube::kSolvedCube,
                       kThistlethwaitePhase4Turns);
}

} // namespace cubot
