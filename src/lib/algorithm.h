#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <string>
#include <vector>

namespace cubot {

class Algorithm {
  public:
    enum struct Turn {
        kUpClockwise,
        kUpHalf,
        kUpCounterclockwise,
        kDownClockwise,
        kDownHalf,
        kDownCounterclockwise,
        kRightClockwise,
        kRightHalf,
        kRightCounterclockwise,
        kLeftClockwise,
        kLeftHalf,
        kLeftCounterclockwise,
        kFrontClockwise,
        kFrontHalf,
        kFrontCounterclockwise,
        kBackClockwise,
        kBackHalf,
        kBackCounterclockwise
    };

    Algorithm();
    Algorithm(std::vector<enum Turn> turns);
    Algorithm(std::string stringNotation);

    std::string ToStringNotation();

    const std::vector<enum Turn> &turns();

  private:
    std::vector<enum Turn> turns_;
};

} // namespace cubot

#endif
