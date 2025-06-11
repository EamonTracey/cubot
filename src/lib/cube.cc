#include "cube.h"

#include <array>
#include <string>
#include <unordered_map>

using namespace cubot;

static constexpr int kUpFront =
    static_cast<int>(Cube::Edge::Position::kUpFront);
static constexpr int kUpBack = static_cast<int>(Cube::Edge::Position::kUpBack);
static constexpr int kUpRight =
    static_cast<int>(Cube::Edge::Position::kUpRight);
static constexpr int kUpLeft = static_cast<int>(Cube::Edge::Position::kUpLeft);
static constexpr int kDownFront =
    static_cast<int>(Cube::Edge::Position::kDownFront);
static constexpr int kDownBack =
    static_cast<int>(Cube::Edge::Position::kDownBack);
static constexpr int kDownRight =
    static_cast<int>(Cube::Edge::Position::kDownRight);
static constexpr int kDownLeft =
    static_cast<int>(Cube::Edge::Position::kDownLeft);
static constexpr int kRightFront =
    static_cast<int>(Cube::Edge::Position::kRightFront);
static constexpr int kLeftFront =
    static_cast<int>(Cube::Edge::Position::kLeftFront);
static constexpr int kRightBack =
    static_cast<int>(Cube::Edge::Position::kRightBack);
static constexpr int kLeftBack =
    static_cast<int>(Cube::Edge::Position::kLeftBack);

static constexpr int kUpRightFront =
    static_cast<int>(Cube::Corner::Position::kUpRightFront);
static constexpr int kUpLeftFront =
    static_cast<int>(Cube::Corner::Position::kUpLeftFront);
static constexpr int kUpRightBack =
    static_cast<int>(Cube::Corner::Position::kUpRightBack);
static constexpr int kUpLeftBack =
    static_cast<int>(Cube::Corner::Position::kUpLeftBack);
static constexpr int kDownRightFront =
    static_cast<int>(Cube::Corner::Position::kDownRightFront);
static constexpr int kDownLeftFront =
    static_cast<int>(Cube::Corner::Position::kDownLeftFront);
static constexpr int kDownRightBack =
    static_cast<int>(Cube::Corner::Position::kDownRightBack);
static constexpr int kDownLeftBack =
    static_cast<int>(Cube::Corner::Position::kDownLeftBack);

static_assert(static_cast<int>(Cube::Edge::Orientation::kCorrect) ==
              static_cast<int>(Cube::Corner::Orientation::kCorrect));
// static constexpr int kCorrect =
//     static_cast<int>(Cube::Edge::Orientation::kCorrect);
static constexpr int kFlipped =
    static_cast<int>(Cube::Edge::Orientation::kFlipped);
static constexpr int kClockwise =
    static_cast<int>(Cube::Corner::Orientation::kClockwise);
static constexpr int kCounterclockwise =
    static_cast<int>(Cube::Corner::Orientation::kCounterclockwise);

typedef void (Cube::*TurnMethod)();
static const TurnMethod kTurnToMethod[] = {&Cube::TurnUpClockwise,
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

static const std::array<Cube::Edge, 12> kSolvedEdges = {
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

static const std::array<Cube::Corner, 8> kSolvedCorners = {
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

static constexpr Cube::Edge::Orientation kFlipEdge[2][2] = {
    {Cube::Edge::Orientation::kCorrect, Cube::Edge::Orientation::kFlipped},
    {Cube::Edge::Orientation::kFlipped, Cube::Edge::Orientation::kCorrect}};
static constexpr Cube::Corner::Orientation kTwistCorner[3][3] = {
    {Cube::Corner::Orientation::kCorrect, Cube::Corner::Orientation::kClockwise,
     Cube::Corner::Orientation::kCounterclockwise},
    {Cube::Corner::Orientation::kClockwise,
     Cube::Corner::Orientation::kCounterclockwise,
     Cube::Corner::Orientation::kCorrect},
    {Cube::Corner::Orientation::kCounterclockwise,
     Cube::Corner::Orientation::kCorrect,
     Cube::Corner::Orientation::kClockwise}};

template <typename T> static void cycle2(T &a, T &b) {
    T tempA = a;
    a = b;
    b = tempA;
}

template <typename T> static void cycle4(T &a, T &b, T &c, T &d) {
    T tempA = a;
    a = b;
    b = c;
    c = d;
    d = tempA;
}

namespace cubot {

const Cube Cube::kSolvedCube = Cube();

Cube::Cube() {
    edges_ = kSolvedEdges;
    corners_ = kSolvedCorners;
}

void Cube::TurnUpClockwise() {
    cycle4(edges_[kUpRight], edges_[kUpBack], edges_[kUpLeft],
           edges_[kUpFront]);
    cycle4(corners_[kUpRightFront], corners_[kUpRightBack],
           corners_[kUpLeftBack], corners_[kUpLeftFront]);
}

void Cube::TurnUpHalf() {
    cycle2(edges_[kUpRight], edges_[kUpLeft]);
    cycle2(edges_[kUpFront], edges_[kUpBack]);
    cycle2(corners_[kUpRightFront], corners_[kUpLeftBack]);
    cycle2(corners_[kUpLeftFront], corners_[kUpRightBack]);
}

void Cube::TurnUpCounterclockwise() {
    cycle4(edges_[kUpRight], edges_[kUpFront], edges_[kUpLeft],
           edges_[kUpBack]);
    cycle4(corners_[kUpRightFront], corners_[kUpLeftFront],
           corners_[kUpLeftBack], corners_[kUpRightBack]);
}

void Cube::TurnDownClockwise() {
    cycle4(edges_[kDownRight], edges_[kDownFront], edges_[kDownLeft],
           edges_[kDownBack]);
    cycle4(corners_[kDownRightFront], corners_[kDownLeftFront],
           corners_[kDownLeftBack], corners_[kDownRightBack]);
}

void Cube::TurnDownHalf() {
    cycle2(edges_[kDownRight], edges_[kDownLeft]);
    cycle2(edges_[kDownFront], edges_[kDownBack]);
    cycle2(corners_[kDownRightFront], corners_[kDownLeftBack]);
    cycle2(corners_[kDownLeftFront], corners_[kDownRightBack]);
}

void Cube::TurnDownCounterclockwise() {
    cycle4(edges_[kDownRight], edges_[kDownBack], edges_[kDownLeft],
           edges_[kDownFront]);
    cycle4(corners_[kDownRightFront], corners_[kDownRightBack],
           corners_[kDownLeftBack], corners_[kDownLeftFront]);
}

void Cube::TurnRightClockwise() {
    cycle4(edges_[kUpRight], edges_[kRightFront], edges_[kDownRight],
           edges_[kRightBack]);
    cycle4(corners_[kUpRightFront], corners_[kDownRightFront],
           corners_[kDownRightBack], corners_[kUpRightBack]);

    corners_[kUpRightFront].orientation =
        kTwistCorner[static_cast<int>(corners_[kUpRightFront].orientation)]
                    [kCounterclockwise];
    corners_[kUpRightBack].orientation =
        kTwistCorner[static_cast<int>(corners_[kUpRightBack].orientation)]
                    [kClockwise];
    corners_[kDownRightFront].orientation =
        kTwistCorner[static_cast<int>(corners_[kDownRightFront].orientation)]
                    [kClockwise];
    corners_[kDownRightBack].orientation =
        kTwistCorner[static_cast<int>(corners_[kDownRightBack].orientation)]
                    [kCounterclockwise];
}

void Cube::TurnRightHalf() {
    cycle2(edges_[kUpRight], edges_[kDownRight]);
    cycle2(edges_[kRightFront], edges_[kRightBack]);
    cycle2(corners_[kUpRightFront], corners_[kDownRightBack]);
    cycle2(corners_[kUpRightBack], corners_[kDownRightFront]);
}

void Cube::TurnRightCounterclockwise() {
    cycle4(edges_[kUpRight], edges_[kRightBack], edges_[kDownRight],
           edges_[kRightFront]);
    cycle4(corners_[kUpRightFront], corners_[kUpRightBack],
           corners_[kDownRightBack], corners_[kDownRightFront]);

    corners_[kUpRightFront].orientation =
        kTwistCorner[static_cast<int>(corners_[kUpRightFront].orientation)]
                    [kCounterclockwise];
    corners_[kUpRightBack].orientation =
        kTwistCorner[static_cast<int>(corners_[kUpRightBack].orientation)]
                    [kClockwise];
    corners_[kDownRightFront].orientation =
        kTwistCorner[static_cast<int>(corners_[kDownRightFront].orientation)]
                    [kClockwise];
    corners_[kDownRightBack].orientation =
        kTwistCorner[static_cast<int>(corners_[kDownRightBack].orientation)]
                    [kCounterclockwise];
}

void Cube::TurnLeftClockwise() {
    cycle4(edges_[kUpLeft], edges_[kLeftBack], edges_[kDownLeft],
           edges_[kLeftFront]);
    cycle4(corners_[kUpLeftFront], corners_[kUpLeftBack],
           corners_[kDownLeftBack], corners_[kDownLeftFront]);

    corners_[kUpLeftFront].orientation =
        kTwistCorner[static_cast<int>(corners_[kUpLeftFront].orientation)]
                    [kClockwise];
    corners_[kUpLeftBack].orientation =
        kTwistCorner[static_cast<int>(corners_[kUpLeftBack].orientation)]
                    [kCounterclockwise];
    corners_[kDownLeftFront].orientation =
        kTwistCorner[static_cast<int>(corners_[kDownLeftFront].orientation)]
                    [kCounterclockwise];
    corners_[kDownLeftBack].orientation =
        kTwistCorner[static_cast<int>(corners_[kDownLeftBack].orientation)]
                    [kClockwise];
}

void Cube::TurnLeftHalf() {
    cycle2(edges_[kUpLeft], edges_[kDownLeft]);
    cycle2(edges_[kLeftFront], edges_[kLeftBack]);
    cycle2(corners_[kUpLeftFront], corners_[kDownLeftBack]);
    cycle2(corners_[kUpLeftBack], corners_[kDownLeftFront]);
}

void Cube::TurnLeftCounterclockwise() {
    cycle4(edges_[kUpLeft], edges_[kLeftFront], edges_[kDownLeft],
           edges_[kLeftBack]);
    cycle4(corners_[kUpLeftFront], corners_[kDownLeftFront],
           corners_[kDownLeftBack], corners_[kUpLeftBack]);

    corners_[kUpLeftFront].orientation =
        kTwistCorner[static_cast<int>(corners_[kUpLeftFront].orientation)]
                    [kClockwise];
    corners_[kUpLeftBack].orientation =
        kTwistCorner[static_cast<int>(corners_[kUpLeftBack].orientation)]
                    [kCounterclockwise];
    corners_[kDownLeftFront].orientation =
        kTwistCorner[static_cast<int>(corners_[kDownLeftFront].orientation)]
                    [kCounterclockwise];
    corners_[kDownLeftBack].orientation =
        kTwistCorner[static_cast<int>(corners_[kDownLeftBack].orientation)]
                    [kClockwise];
}

void Cube::TurnFrontClockwise() {
    cycle4(edges_[kUpFront], edges_[kLeftFront], edges_[kDownFront],
           edges_[kRightFront]);
    cycle4(corners_[kUpRightFront], corners_[kUpLeftFront],
           corners_[kDownLeftFront], corners_[kDownRightFront]);

    edges_[kUpFront].orientation =
        kFlipEdge[static_cast<int>(edges_[kUpFront].orientation)][kFlipped];
    edges_[kDownFront].orientation =
        kFlipEdge[static_cast<int>(edges_[kDownFront].orientation)][kFlipped];
    edges_[kRightFront].orientation =
        kFlipEdge[static_cast<int>(edges_[kRightFront].orientation)][kFlipped];
    edges_[kLeftFront].orientation =
        kFlipEdge[static_cast<int>(edges_[kLeftFront].orientation)][kFlipped];
    corners_[kUpRightFront].orientation =
        kTwistCorner[static_cast<int>(corners_[kUpRightFront].orientation)]
                    [kClockwise];
    corners_[kUpLeftFront].orientation =
        kTwistCorner[static_cast<int>(corners_[kUpLeftFront].orientation)]
                    [kCounterclockwise];
    corners_[kDownRightFront].orientation =
        kTwistCorner[static_cast<int>(corners_[kDownRightFront].orientation)]
                    [kCounterclockwise];
    corners_[kDownLeftFront].orientation =
        kTwistCorner[static_cast<int>(corners_[kDownLeftFront].orientation)]
                    [kClockwise];
}

void Cube::TurnFrontHalf() {
    cycle2(edges_[kUpFront], edges_[kDownFront]);
    cycle2(edges_[kRightFront], edges_[kLeftFront]);
    cycle2(corners_[kUpRightFront], corners_[kDownLeftFront]);
    cycle2(corners_[kUpLeftFront], corners_[kDownRightFront]);
}

void Cube::TurnFrontCounterclockwise() {
    cycle4(edges_[kUpFront], edges_[kRightFront], edges_[kDownFront],
           edges_[kLeftFront]);
    cycle4(corners_[kUpRightFront], corners_[kDownRightFront],
           corners_[kDownLeftFront], corners_[kUpLeftFront]);

    edges_[kUpFront].orientation =
        kFlipEdge[static_cast<int>(edges_[kUpFront].orientation)][kFlipped];
    edges_[kDownFront].orientation =
        kFlipEdge[static_cast<int>(edges_[kDownFront].orientation)][kFlipped];
    edges_[kRightFront].orientation =
        kFlipEdge[static_cast<int>(edges_[kRightFront].orientation)][kFlipped];
    edges_[kLeftFront].orientation =
        kFlipEdge[static_cast<int>(edges_[kLeftFront].orientation)][kFlipped];
    corners_[kUpRightFront].orientation =
        kTwistCorner[static_cast<int>(corners_[kUpRightFront].orientation)]
                    [kClockwise];
    corners_[kUpLeftFront].orientation =
        kTwistCorner[static_cast<int>(corners_[kUpLeftFront].orientation)]
                    [kCounterclockwise];
    corners_[kDownRightFront].orientation =
        kTwistCorner[static_cast<int>(corners_[kDownRightFront].orientation)]
                    [kCounterclockwise];
    corners_[kDownLeftFront].orientation =
        kTwistCorner[static_cast<int>(corners_[kDownLeftFront].orientation)]
                    [kClockwise];
}

void Cube::TurnBackClockwise() {
    cycle4(edges_[kUpBack], edges_[kRightBack], edges_[kDownBack],
           edges_[kLeftBack]);
    cycle4(corners_[kUpRightBack], corners_[kDownRightBack],
           corners_[kDownLeftBack], corners_[kUpLeftBack]);

    edges_[kUpBack].orientation =
        kFlipEdge[static_cast<int>(edges_[kUpBack].orientation)][kFlipped];
    edges_[kDownBack].orientation =
        kFlipEdge[static_cast<int>(edges_[kDownBack].orientation)][kFlipped];
    edges_[kRightBack].orientation =
        kFlipEdge[static_cast<int>(edges_[kRightBack].orientation)][kFlipped];
    edges_[kLeftBack].orientation =
        kFlipEdge[static_cast<int>(edges_[kLeftBack].orientation)][kFlipped];
    corners_[kUpRightBack].orientation =
        kTwistCorner[static_cast<int>(corners_[kUpRightBack].orientation)]
                    [kCounterclockwise];
    corners_[kUpLeftBack].orientation =
        kTwistCorner[static_cast<int>(corners_[kUpLeftBack].orientation)]
                    [kClockwise];
    corners_[kDownRightBack].orientation =
        kTwistCorner[static_cast<int>(corners_[kDownRightBack].orientation)]
                    [kClockwise];
    corners_[kDownLeftBack].orientation =
        kTwistCorner[static_cast<int>(corners_[kDownLeftBack].orientation)]
                    [kCounterclockwise];
}

void Cube::TurnBackHalf() {
    cycle2(edges_[kUpBack], edges_[kDownBack]);
    cycle2(edges_[kRightBack], edges_[kLeftBack]);
    cycle2(corners_[kUpRightBack], corners_[kDownLeftBack]);
    cycle2(corners_[kUpLeftBack], corners_[kDownRightBack]);
}

void Cube::TurnBackCounterclockwise() {
    cycle4(edges_[kUpBack], edges_[kLeftBack], edges_[kDownBack],
           edges_[kRightBack]);
    cycle4(corners_[kUpRightBack], corners_[kUpLeftBack],
           corners_[kDownLeftBack], corners_[kDownRightBack]);

    edges_[kUpBack].orientation =
        kFlipEdge[static_cast<int>(edges_[kUpBack].orientation)][kFlipped];
    edges_[kDownBack].orientation =
        kFlipEdge[static_cast<int>(edges_[kDownBack].orientation)][kFlipped];
    edges_[kRightBack].orientation =
        kFlipEdge[static_cast<int>(edges_[kRightBack].orientation)][kFlipped];
    edges_[kLeftBack].orientation =
        kFlipEdge[static_cast<int>(edges_[kLeftBack].orientation)][kFlipped];
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

    auto &edges_to_apply = cube.edges();
    auto &corners_to_apply = cube.corners();
    for (size_t i = 0; i < 12; ++i) {
        new_edges[i] =
            edges_[static_cast<size_t>(edges_to_apply[i].solvedPosition)];
        new_edges[i].orientation =
            kFlipEdge[static_cast<int>(new_edges[i].orientation)]
                     [static_cast<int>(edges_to_apply[i].orientation)];

        if (i < 8) {
            new_corners[i] = corners_[static_cast<size_t>(
                corners_to_apply[i].solvedPosition)];
            new_corners[i].orientation =
                kTwistCorner[static_cast<int>(new_corners[i].orientation)]
                            [static_cast<int>(corners_to_apply[i].orientation)];
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

} // namespace cubot
