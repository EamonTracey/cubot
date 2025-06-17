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
            kRightBack,
            kLeftFront,
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

    enum struct Facelet { kUp, kDown, kRight, kLeft, kFront, kBack };

    static const Cube kSolvedCube;

    Cube();
    Cube(const std::array<struct Edge, 12> &edges,
         const std::array<struct Corner, 8> &corners);
    Cube(const std::array<enum Facelet, 54> &facelets);
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

    bool IsValid() const;
    bool AreEdgesOrientable() const;
    bool AreCornersOrientable() const;
    bool IsPermutable() const;
    bool IsSolvable() const;

    std::array<enum Facelet, 54> ToFacelets() const;

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
