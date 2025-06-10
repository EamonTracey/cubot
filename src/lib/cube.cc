#include "cube.h"

#include <string>
#include <unordered_map>

constexpr int upFront = static_cast<int>(Cube::Edge::Position::upFront);
constexpr int upBack = static_cast<int>(Cube::Edge::Position::upBack);
constexpr int upRight = static_cast<int>(Cube::Edge::Position::upRight);
constexpr int upLeft = static_cast<int>(Cube::Edge::Position::upLeft);
constexpr int downFront = static_cast<int>(Cube::Edge::Position::downFront);
constexpr int downBack = static_cast<int>(Cube::Edge::Position::downBack);
constexpr int downRight = static_cast<int>(Cube::Edge::Position::downRight);
constexpr int downLeft = static_cast<int>(Cube::Edge::Position::downLeft);
constexpr int rightFront = static_cast<int>(Cube::Edge::Position::rightFront);
constexpr int leftFront = static_cast<int>(Cube::Edge::Position::leftFront);
constexpr int rightBack = static_cast<int>(Cube::Edge::Position::rightBack);
constexpr int leftBack = static_cast<int>(Cube::Edge::Position::leftBack);

constexpr int upRightFront =
    static_cast<int>(Cube::Corner::Position::upRightFront);
constexpr int upLeftFront =
    static_cast<int>(Cube::Corner::Position::upLeftFront);
constexpr int upRightBack =
    static_cast<int>(Cube::Corner::Position::upRightBack);
constexpr int upLeftBack = static_cast<int>(Cube::Corner::Position::upLeftBack);
constexpr int downRightFront =
    static_cast<int>(Cube::Corner::Position::downRightFront);
constexpr int downLeftFront =
    static_cast<int>(Cube::Corner::Position::downLeftFront);
constexpr int downRightBack =
    static_cast<int>(Cube::Corner::Position::downRightBack);
constexpr int downLeftBack =
    static_cast<int>(Cube::Corner::Position::downLeftBack);

const std::unordered_map<enum Algorithm::Turn, void (Cube::*)()> kTurnToMethod =
    {{Algorithm::Turn::upClockwise, &Cube::TurnUpClockwise},
     {Algorithm::Turn::upHalf, &Cube::TurnUpHalf},
     {Algorithm::Turn::upCounterclockwise, &Cube::TurnUpCounterclockwise},
     {Algorithm::Turn::downClockwise, &Cube::TurnDownClockwise},
     {Algorithm::Turn::downHalf, &Cube::TurnDownHalf},
     {Algorithm::Turn::downCounterclockwise, &Cube::TurnDownCounterclockwise},
     {Algorithm::Turn::rightClockwise, &Cube::TurnRightClockwise},
     {Algorithm::Turn::rightHalf, &Cube::TurnRightHalf},
     {Algorithm::Turn::rightCounterclockwise, &Cube::TurnRightCounterclockwise},
     {Algorithm::Turn::leftClockwise, &Cube::TurnLeftClockwise},
     {Algorithm::Turn::leftHalf, &Cube::TurnLeftHalf},
     {Algorithm::Turn::leftCounterclockwise, &Cube::TurnLeftCounterclockwise},
     {Algorithm::Turn::frontClockwise, &Cube::TurnFrontClockwise},
     {Algorithm::Turn::frontHalf, &Cube::TurnFrontHalf},
     {Algorithm::Turn::frontCounterclockwise, &Cube::TurnFrontCounterclockwise},
     {Algorithm::Turn::backClockwise, &Cube::TurnBackClockwise},
     {Algorithm::Turn::backHalf, &Cube::TurnBackHalf},
     {Algorithm::Turn::backCounterclockwise, &Cube::TurnBackCounterclockwise}};

constexpr int flip_edge[2][2] = {{0, 1}, {1, 0}};
constexpr int twist_corner[3][3] = {{0, 1, 2}, {1, 2, 0}, {2, 0, 1}};

template <typename T> void cycle2(T &a, T &b) {
    T tempA = a;
    a = b;
    b = tempA;
}

template <typename T> void cycle4(T &a, T &b, T &c, T &d) {
    T tempA = a;
    a = b;
    b = c;
    c = d;
    d = tempA;
}

const Cube Cube::kSolvedCube = Cube();

Cube::Cube() {
    edges_ = {Edge{Edge::Orientation::correct, Edge::Position::upRight},
              Edge{Edge::Orientation::correct, Edge::Position::upLeft},
              Edge{Edge::Orientation::correct, Edge::Position::upFront},
              Edge{Edge::Orientation::correct, Edge::Position::upBack},
              Edge{Edge::Orientation::correct, Edge::Position::downRight},
              Edge{Edge::Orientation::correct, Edge::Position::downLeft},
              Edge{Edge::Orientation::correct, Edge::Position::downFront},
              Edge{Edge::Orientation::correct, Edge::Position::downBack},
              Edge{Edge::Orientation::correct, Edge::Position::rightFront},
              Edge{Edge::Orientation::correct, Edge::Position::leftFront},
              Edge{Edge::Orientation::correct, Edge::Position::rightBack},
              Edge{Edge::Orientation::correct, Edge::Position::leftBack}};
    corners_ = {
        Corner{Corner::Orientation::correct, Corner::Position::upRightFront},
        Corner{Corner::Orientation::correct, Corner::Position::upRightBack},
        Corner{Corner::Orientation::correct, Corner::Position::upLeftFront},
        Corner{Corner::Orientation::correct, Corner::Position::upLeftBack},
        Corner{Corner::Orientation::correct, Corner::Position::downRightFront},
        Corner{Corner::Orientation::correct, Corner::Position::downRightBack},
        Corner{Corner::Orientation::correct, Corner::Position::downLeftFront},
        Corner{Corner::Orientation::correct, Corner::Position::downLeftBack}};
}

void Cube::TurnUpClockwise() {
    cycle4(edges_[upRight], edges_[upBack], edges_[upLeft], edges_[upFront]);
    cycle4(corners_[upRightFront], corners_[upRightBack], corners_[upLeftBack],
           corners_[upLeftFront]);
}

void Cube::TurnUpHalf() {
    cycle2(edges_[upRight], edges_[upLeft]);
    cycle2(edges_[upFront], edges_[upBack]);
    cycle2(corners_[upRightFront], corners_[upLeftBack]);
    cycle2(corners_[upLeftFront], corners_[upRightBack]);
}

void Cube::TurnUpCounterclockwise() {
    cycle4(edges_[upRight], edges_[upFront], edges_[upLeft], edges_[upBack]);
    cycle4(corners_[upRightFront], corners_[upLeftFront], corners_[upLeftBack],
           corners_[upRightBack]);
}

void Cube::TurnDownClockwise() {
    cycle4(edges_[downRight], edges_[downFront], edges_[downLeft],
           edges_[downBack]);
    cycle4(corners_[downRightFront], corners_[downLeftFront],
           corners_[downLeftBack], corners_[downRightBack]);
}

void Cube::TurnDownHalf() {
    cycle2(edges_[downRight], edges_[downLeft]);
    cycle2(edges_[downFront], edges_[downBack]);
    cycle2(corners_[downRightFront], corners_[downLeftBack]);
    cycle2(corners_[downLeftFront], corners_[downRightBack]);
}

void Cube::TurnDownCounterclockwise() {
    cycle4(edges_[downRight], edges_[downBack], edges_[downLeft],
           edges_[downFront]);
    cycle4(corners_[downRightFront], corners_[downRightBack],
           corners_[downLeftBack], corners_[downLeftFront]);
}

void Cube::TurnRightClockwise() {
    cycle4(edges_[upRight], edges_[rightFront], edges_[downRight],
           edges_[rightBack]);
    cycle4(corners_[upRightFront], corners_[downRightFront],
           corners_[downRightBack], corners_[upRightBack]);
}

void Cube::TurnRightHalf() {
    cycle2(edges_[upRight], edges_[downRight]);
    cycle2(edges_[rightFront], edges_[rightBack]);
    cycle2(corners_[upRightFront], corners_[downRightBack]);
    cycle2(corners_[upRightBack], corners_[downRightFront]);
}

void Cube::TurnRightCounterclockwise() {
    cycle4(edges_[upRight], edges_[rightBack], edges_[downRight],
           edges_[rightFront]);
    cycle4(corners_[upRightFront], corners_[upRightBack],
           corners_[downRightBack], corners_[downRightFront]);
}

void Cube::TurnLeftClockwise() {
    cycle4(edges_[upLeft], edges_[leftBack], edges_[downLeft],
           edges_[leftFront]);
    cycle4(corners_[upLeftFront], corners_[upLeftBack], corners_[downLeftBack],
           corners_[downLeftFront]);
}

void Cube::TurnLeftHalf() {
    cycle2(edges_[upLeft], edges_[downLeft]);
    cycle2(edges_[leftFront], edges_[leftBack]);
    cycle2(corners_[upLeftFront], corners_[downLeftBack]);
    cycle2(corners_[upLeftBack], corners_[downLeftFront]);
}

void Cube::TurnLeftCounterclockwise() {
    cycle4(edges_[upLeft], edges_[leftFront], edges_[downLeft],
           edges_[leftBack]);
    cycle4(corners_[upLeftFront], corners_[downLeftFront],
           corners_[downLeftBack], corners_[upLeftBack]);
}

void Cube::TurnFrontClockwise() {
    cycle4(edges_[upFront], edges_[leftFront], edges_[downFront],
           edges_[rightFront]);
    cycle4(corners_[upRightFront], corners_[upLeftFront],
           corners_[downLeftFront], corners_[downRightFront]);
}

void Cube::TurnFrontHalf() {
    cycle2(edges_[upFront], edges_[downFront]);
    cycle2(edges_[rightFront], edges_[leftFront]);
    cycle2(corners_[upRightFront], corners_[downLeftFront]);
    cycle2(corners_[upLeftFront], corners_[downRightFront]);
}

void Cube::TurnFrontCounterclockwise() {
    cycle4(edges_[upFront], edges_[rightFront], edges_[downFront],
           edges_[leftFront]);
    cycle4(corners_[upRightFront], corners_[downRightFront],
           corners_[downLeftFront], corners_[upLeftFront]);
}

void Cube::TurnBackClockwise() {
    cycle4(edges_[upBack], edges_[rightBack], edges_[downBack],
           edges_[leftBack]);
    cycle4(corners_[upRightBack], corners_[downRightBack],
           corners_[downLeftBack], corners_[upLeftBack]);
}

void Cube::TurnBackHalf() {
    cycle2(edges_[upBack], edges_[downBack]);
    cycle2(edges_[rightFront], edges_[leftFront]);
    cycle2(corners_[upRightBack], corners_[downLeftBack]);
    cycle2(corners_[upLeftBack], corners_[downRightBack]);
}

void Cube::TurnBackCounterclockwise() {
    cycle4(edges_[upBack], edges_[leftBack], edges_[downBack],
           edges_[rightBack]);
    cycle4(corners_[upRightBack], corners_[upLeftBack], corners_[downLeftBack],
           corners_[downRightBack]);
}

void Cube::Execute(Algorithm algorithm) {
    for (auto turn : algorithm.turns())
        Execute(turn);
}

void Cube::Execute(Algorithm::Turn turn) {
    auto method = kTurnToMethod.at(turn);
    (this->*method)();
}

void Cube::Apply(const Cube &cube) {
    std::array<struct Cube::Edge, 12> new_edges;
    std::array<struct Cube::Corner, 8> new_corners;

    auto edges_to_apply = cube.edges();
    auto corners_to_apply = cube.corners();
    for (size_t i = 0; i < 12; ++i) {
        new_edges[i] =
            edges_[static_cast<size_t>(edges_to_apply[i].solvedPosition)];
        new_edges[i].orientation = static_cast<enum Cube::Edge::Orientation>(
            flip_edge[static_cast<int>(new_edges[i].orientation)]
                     [static_cast<int>(edges_to_apply[i].orientation)]);

        if (i < 8) {
            new_corners[i] = corners_[static_cast<size_t>(
                corners_to_apply[i].solvedPosition)];
            new_corners[i].orientation =
                static_cast<enum Cube::Corner::Orientation>(
                    twist_corner[static_cast<int>(new_corners[i].orientation)]
                                [static_cast<int>(
                                    corners_to_apply[i].orientation)]);
        }
    }

    edges_ = new_edges;
    corners_ = new_corners;
}

const std::array<struct Cube::Edge, 12> &Cube::edges() const { return edges_; }

const std::array<struct Cube::Corner, 8> &Cube::corners() const {
    return corners_;
}

bool operator==(const Cube::Edge &a, const Cube::Edge &b) {
    return a.orientation == b.orientation &&
           a.solvedPosition == b.solvedPosition;
}

bool operator==(const Cube::Corner &a, const Cube::Corner &b) {
    return a.orientation == b.orientation &&
           a.solvedPosition == b.solvedPosition;
}

bool operator==(const Cube &a, const Cube &b) {
    return a.edges() == b.edges() && a.corners() == b.corners();
}
