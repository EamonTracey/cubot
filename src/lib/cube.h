#ifndef CUBE_H
#define CUBE_H

#include <array>

#include "algorithm.h"

namespace cubot {

class Cube {
  public:
    struct Edge {
        enum struct Orientation { kCorrect, kFlipped };
        enum struct Position {
            kUpRight,
            kUpLeft,
            kUpFront,
            kUpBack,
            kDownRight,
            kDownLeft,
            kDownFront,
            kDownBack,
            kRightFront,
            kLeftFront,
            kRightBack,
            kLeftBack
        };
        enum Orientation orientation;
        enum Position solved_position;
    };

    struct Corner {
        enum struct Orientation { kCorrect, kClockwise, kCounterclockwise };
        enum struct Position {
            kUpRightFront,
            kUpRightBack,
            kUpLeftFront,
            kUpLeftBack,
            kDownRightFront,
            kDownRightBack,
            kDownLeftFront,
            kDownLeftBack
        };
        enum Orientation orientation;
        enum Position solved_position;
    };

    static const Cube kSolvedCube;

    Cube();
    Cube(const std::array<struct Edge, 12> &edges,
         const std::array<struct Corner, 8> &corners);
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
    void Invert();

    const std::array<struct Edge, 12> &edges() const;
    const std::array<struct Corner, 8> &corners() const;

  private:
    std::array<struct Edge, 12> edges_;
    std::array<struct Corner, 8> corners_;
};

bool operator==(const Cube::Edge &a, const Cube::Edge &b);
bool operator==(const Cube::Corner &a, const Cube::Corner &b);
bool operator==(const Cube &a, const Cube &b);

} // namespace cubot

#endif
