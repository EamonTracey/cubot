#include "algorithm.h"

#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

const std::unordered_map<std::string, enum Algorithm::Turn> kStringToTurn = {
    {"U", Algorithm::Turn::kUpClockwise},
    {"U2", Algorithm::Turn::kUpHalf},
    {"U'", Algorithm::Turn::kUpCounterclockwise},
    {"D", Algorithm::Turn::kDownClockwise},
    {"D2", Algorithm::Turn::kDownHalf},
    {"D'", Algorithm::Turn::kDownCounterclockwise},
    {"R", Algorithm::Turn::kRightClockwise},
    {"R2", Algorithm::Turn::kRightHalf},
    {"R'", Algorithm::Turn::kRightCounterclockwise},
    {"L", Algorithm::Turn::kLeftClockwise},
    {"L2", Algorithm::Turn::kLeftHalf},
    {"L'", Algorithm::Turn::kLeftCounterclockwise},
    {"F", Algorithm::Turn::kFrontClockwise},
    {"F2", Algorithm::Turn::kFrontHalf},
    {"F'", Algorithm::Turn::kFrontCounterclockwise},
    {"B", Algorithm::Turn::kBackClockwise},
    {"B2", Algorithm::Turn::kBackHalf},
    {"B'", Algorithm::Turn::kBackCounterclockwise}};

const std::string kTurnToString[] = {
    "U", "U2", "U'", "D", "D2", "D'", "R", "R2", "R'",
    "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'",
};

Algorithm::Algorithm(std::vector<enum Algorithm::Turn> turns) : turns_(turns) {}

Algorithm::Algorithm(std::string stringNotation) {
    std::stringstream ss(stringNotation);
    std::string token;
    while (ss >> token)
        turns_.push_back(kStringToTurn.at(token));
}

std::string Algorithm::ToStringNotation() {
    std::string stringNotation;

    for (size_t i = 0; i < turns_.size(); ++i) {
        if (i != 0)
            stringNotation += " ";
        stringNotation += kTurnToString[static_cast<int>(turns_[i])];
    }

    return stringNotation;
}

const std::vector<enum Algorithm::Turn> &Algorithm::turns() { return turns_; }
