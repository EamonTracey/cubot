#ifndef CUBE_H
#define CUBE_H

#include <array>

#include "algorithm.h"

class Cube {
  public:
    struct Edge {
        enum struct Orientation { correct, flipped };
        enum struct Position {
            upRight,
            upLeft,
            upFront,
            upBack,
            downRight,
            downLeft,
            downFront,
            downBack,
            rightFront,
            leftFront,
            rightBack,
            leftBack
        };
        enum Orientation orientation;
        enum Position solvedPosition;
    };

    struct Corner {
        enum struct Orientation { correct, clockwise, counterclockwise };
        enum struct Position {
            upRightFront,
            upRightBack,
            upLeftFront,
            upLeftBack,
            downRightFront,
            downRightBack,
            downLeftFront,
            downLeftBack
        };
        enum Orientation orientation;
        enum Position solvedPosition;
    };

    static const Cube kSolvedCube;

    Cube();
    ~Cube() {}

    void TurnUpClockwise();
    void TurnUpHalf();
    void TurnUpCounterclockwise();
    void TurnDownClockwise();
    void TurnDownHalf();
    void TurnDownCounterclockwise();
    void TurnRightClockwise();
    void TurnRightHalf();
    void TurnRightCounterclockwise();
    void TurnLeftClockwise();
    void TurnLeftHalf();
    void TurnLeftCounterclockwise();
    void TurnFrontClockwise();
    void TurnFrontHalf();
    void TurnFrontCounterclockwise();
    void TurnBackClockwise();
    void TurnBackHalf();
    void TurnBackCounterclockwise();

    void Execute(Algorithm algorithm);
    void Execute(Algorithm::Turn turn);
    void Apply(const Cube &cube);

    const std::array<struct Edge, 12> &edges() const;
    const std::array<struct Corner, 8> &corners() const;

  private:
    std::array<struct Edge, 12> edges_;
    std::array<struct Corner, 8> corners_;
};

bool operator==(const Cube::Edge &a, const Cube::Edge &b);
bool operator==(const Cube::Corner &a, const Cube::Corner &b);
bool operator==(const Cube &a, const Cube &b);

#endif
