#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "algorithm.h"

const std::unordered_map<std::string, enum Algorithm::Turn> kStringToTurn = {
    {"U", Algorithm::Turn::upClockwise},
    {"U2", Algorithm::Turn::upHalf},
    {"U'", Algorithm::Turn::upCounterclockwise},
    {"D", Algorithm::Turn::downClockwise},
    {"D2", Algorithm::Turn::downHalf},
    {"D'", Algorithm::Turn::downCounterclockwise},
    {"R", Algorithm::Turn::rightClockwise},
    {"R2", Algorithm::Turn::rightHalf},
    {"R'", Algorithm::Turn::rightCounterclockwise},
    {"L", Algorithm::Turn::leftClockwise},
    {"L2", Algorithm::Turn::leftHalf},
    {"L'", Algorithm::Turn::leftCounterclockwise},
    {"F", Algorithm::Turn::frontClockwise},
    {"F2", Algorithm::Turn::frontHalf},
    {"F'", Algorithm::Turn::frontCounterclockwise},
    {"B", Algorithm::Turn::backClockwise},
    {"B2", Algorithm::Turn::backHalf},
    {"B'", Algorithm::Turn::backCounterclockwise}};

const std::unordered_map<enum Algorithm::Turn, std::string> kTurnToString = {
    {Algorithm::Turn::upClockwise, "U"},
    {Algorithm::Turn::upHalf, "U2"},
    {Algorithm::Turn::upCounterclockwise, "U'"},
    {Algorithm::Turn::downClockwise, "D"},
    {Algorithm::Turn::downHalf, "D2"},
    {Algorithm::Turn::downCounterclockwise, "D'"},
    {Algorithm::Turn::rightClockwise, "R"},
    {Algorithm::Turn::rightHalf, "R2"},
    {Algorithm::Turn::rightCounterclockwise, "R'"},
    {Algorithm::Turn::leftClockwise, "L"},
    {Algorithm::Turn::leftHalf, "L2"},
    {Algorithm::Turn::leftCounterclockwise, "L'"},
    {Algorithm::Turn::frontClockwise, "F"},
    {Algorithm::Turn::frontHalf, "F2"},
    {Algorithm::Turn::frontCounterclockwise, "F'"},
    {Algorithm::Turn::backClockwise, "B"},
    {Algorithm::Turn::backHalf, "B2"},
    {Algorithm::Turn::backCounterclockwise, "B'"}};

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
        stringNotation += kTurnToString.at(turns_[i]);
    }

    return stringNotation;
}

const std::vector<enum Algorithm::Turn> &Algorithm::turns() { return turns_; }
