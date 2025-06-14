#include "algorithm.h"

#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace cubot;

static const std::unordered_map<std::string, enum Algorithm::Turn>
    kStringToTurn = {{"U", Algorithm::Turn::kUpClockwise},
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

static const std::string kTurnToString[] = {
    "U", "U2", "U'", "D", "D2", "D'", "R", "R2", "R'",
    "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'",
};

static constexpr int kUp = 0;
static constexpr int kDown = 1;
static constexpr int kRight = 2;
static constexpr int kLeft = 3;
static constexpr int kFront = 4;
static constexpr int kBack = 5;
static constexpr int kLayer[] = {0, 0, 0, 1, 1, 1, 2, 2, 2,
                                 3, 3, 3, 4, 4, 4, 5, 5, 5};

static constexpr int kNone = 0;
static constexpr int kClockwise = 1;
static constexpr int kHalf = 2;
static constexpr int kCounterclockwise = 3;
static constexpr int kDegree[] = {1, 2, 3, 1, 2, 3, 1, 2, 3,
                                  1, 2, 3, 1, 2, 3, 1, 2, 3};

namespace cubot {

Algorithm::Algorithm() : turns_({}) {}

Algorithm::Algorithm(std::vector<enum Algorithm::Turn> turns) : turns_(turns) {}

Algorithm::Algorithm(std::string stringNotation) {
    std::stringstream ss(stringNotation);
    std::string token;
    while (ss >> token)
        turns_.push_back(kStringToTurn.at(token));
}

void Algorithm::Compress() {
    std::vector<enum Algorithm::Turn> compressed_turns;

    size_t i = 0;
    while (i < turns_.size()) {
        int layer = kLayer[static_cast<int>(turns_[i])];
        int degree = kDegree[static_cast<int>(turns_[i])];
        size_t j = i + 1;
        while (j < turns_.size() &&
               layer == kLayer[static_cast<int>(turns_[j])]) {
            degree += kDegree[static_cast<int>(turns_[j])];
            ++j;
        }
        degree %= 4;
        i = j;

        if (degree)
            compressed_turns.push_back(
                static_cast<Algorithm::Turn>(layer * 3 + (degree - 1)));
    }

    turns_ = compressed_turns;
}

std::string Algorithm::ToStringNotation() const {
    std::string stringNotation;

    for (size_t i = 0; i < turns_.size(); ++i) {
        if (i != 0)
            stringNotation += " ";
        stringNotation += kTurnToString[static_cast<int>(turns_[i])];
    }

    return stringNotation;
}

const std::vector<enum Algorithm::Turn> &Algorithm::turns() { return turns_; }

} // namespace cubot
