#include "cube.h"

#include <array>
#include <string>
#include <unordered_map>

constexpr int upFront = static_cast<int>(Cube::Edge::Position::kUpFront);
constexpr int upBack = static_cast<int>(Cube::Edge::Position::kUpBack);
constexpr int upRight = static_cast<int>(Cube::Edge::Position::kUpRight);
constexpr int upLeft = static_cast<int>(Cube::Edge::Position::kUpLeft);
constexpr int downFront = static_cast<int>(Cube::Edge::Position::kDownFront);
constexpr int downBack = static_cast<int>(Cube::Edge::Position::kDownBack);
constexpr int downRight = static_cast<int>(Cube::Edge::Position::kDownRight);
constexpr int downLeft = static_cast<int>(Cube::Edge::Position::kDownLeft);
constexpr int rightFront = static_cast<int>(Cube::Edge::Position::kRightFront);
constexpr int leftFront = static_cast<int>(Cube::Edge::Position::kLeftFront);
constexpr int rightBack = static_cast<int>(Cube::Edge::Position::kRightBack);
constexpr int leftBack = static_cast<int>(Cube::Edge::Position::kLeftBack);

constexpr int upRightFront =
    static_cast<int>(Cube::Corner::Position::kUpRightFront);
constexpr int upLeftFront =
    static_cast<int>(Cube::Corner::Position::kUpLeftFront);
constexpr int upRightBack =
    static_cast<int>(Cube::Corner::Position::kUpRightBack);
constexpr int upLeftBack =
    static_cast<int>(Cube::Corner::Position::kUpLeftBack);
constexpr int downRightFront =
    static_cast<int>(Cube::Corner::Position::kDownRightFront);
constexpr int downLeftFront =
    static_cast<int>(Cube::Corner::Position::kDownLeftFront);
constexpr int downRightBack =
    static_cast<int>(Cube::Corner::Position::kDownRightBack);
constexpr int downLeftBack =
    static_cast<int>(Cube::Corner::Position::kDownLeftBack);

typedef void (Cube::*TurnMethod)();
const TurnMethod kTurnToMethod[] = {&Cube::TurnUpClockwise,
                                    &Cube::TurnUpHalf,
                                    &Cube::TurnUpCounterclockwise,
                                    &Cube::TurnDownClockwise,
                                    &Cube::TurnDownHalf,
                                    &Cube::TurnDownCounterclockwise,
                                    &Cube::TurnRightClockwise,
                                    &Cube::TurnRightHalf,
                                    &Cube::TurnRightCounterclockwise,
                                    &Cube::TurnLeftClockwise,
                                    &Cube::TurnLeftHalf,
                                    &Cube::TurnLeftCounterclockwise,
                                    &Cube::TurnFrontClockwise,
                                    &Cube::TurnFrontHalf,
                                    &Cube::TurnFrontCounterclockwise,
                                    &Cube::TurnBackClockwise,
                                    &Cube::TurnBackHalf,
                                    &Cube::TurnBackCounterclockwise};

const std::array<Cube::Edge, 12> kSolvedEdges = {
    Cube::Edge{Cube::Edge::Orientation::kCorrect,
               Cube::Edge::Position::kUpRight},
    Cube::Edge{Cube::Edge::Orientation::kCorrect,
               Cube::Edge::Position::kUpLeft},
    Cube::Edge{Cube::Edge::Orientation::kCorrect,
               Cube::Edge::Position::kUpFront},
    Cube::Edge{Cube::Edge::Orientation::kCorrect,
               Cube::Edge::Position::kUpBack},
    Cube::Edge{Cube::Edge::Orientation::kCorrect,
               Cube::Edge::Position::kDownRight},
    Cube::Edge{Cube::Edge::Orientation::kCorrect,
               Cube::Edge::Position::kDownLeft},
    Cube::Edge{Cube::Edge::Orientation::kCorrect,
               Cube::Edge::Position::kDownFront},
    Cube::Edge{Cube::Edge::Orientation::kCorrect,
               Cube::Edge::Position::kDownBack},
    Cube::Edge{Cube::Edge::Orientation::kCorrect,
               Cube::Edge::Position::kRightFront},
    Cube::Edge{Cube::Edge::Orientation::kCorrect,
               Cube::Edge::Position::kLeftFront},
    Cube::Edge{Cube::Edge::Orientation::kCorrect,
               Cube::Edge::Position::kRightBack},
    Cube::Edge{Cube::Edge::Orientation::kCorrect,
               Cube::Edge::Position::kLeftBack}};

const std::array<Cube::Corner, 8> kSolvedCorners = {
    Cube::Corner{Cube::Corner::Orientation::kCorrect,
                 Cube::Corner::Position::kUpRightFront},
    Cube::Corner{Cube::Corner::Orientation::kCorrect,
                 Cube::Corner::Position::kUpRightBack},
    Cube::Corner{Cube::Corner::Orientation::kCorrect,
                 Cube::Corner::Position::kUpLeftFront},
    Cube::Corner{Cube::Corner::Orientation::kCorrect,
                 Cube::Corner::Position::kUpLeftBack},
    Cube::Corner{Cube::Corner::Orientation::kCorrect,
                 Cube::Corner::Position::kDownRightFront},
    Cube::Corner{Cube::Corner::Orientation::kCorrect,
                 Cube::Corner::Position::kDownRightBack},
    Cube::Corner{Cube::Corner::Orientation::kCorrect,
                 Cube::Corner::Position::kDownLeftFront},
    Cube::Corner{Cube::Corner::Orientation::kCorrect,
                 Cube::Corner::Position::kDownLeftBack}};

constexpr int kFlipEdge[2][2] = {{0, 1}, {1, 0}};
constexpr int kTwistCorner[3][3] = {{0, 1, 2}, {1, 2, 0}, {2, 0, 1}};

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
    edges_ = kSolvedEdges;
    corners_ = kSolvedCorners;
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
    auto method = kTurnToMethod[static_cast<int>(turn)];
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
            kFlipEdge[static_cast<int>(new_edges[i].orientation)]
                     [static_cast<int>(edges_to_apply[i].orientation)]);

        if (i < 8) {
            new_corners[i] = corners_[static_cast<size_t>(
                corners_to_apply[i].solvedPosition)];
            new_corners[i].orientation =
                static_cast<enum Cube::Corner::Orientation>(
                    kTwistCorner[static_cast<int>(new_corners[i].orientation)]
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
