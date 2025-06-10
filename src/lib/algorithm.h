#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <string>
#include <vector>

class Algorithm {
  public:
    enum struct Turn {
        upClockwise,
        upHalf,
        upCounterclockwise,
        downClockwise,
        downHalf,
        downCounterclockwise,
        rightClockwise,
        rightHalf,
        rightCounterclockwise,
        leftClockwise,
        leftHalf,
        leftCounterclockwise,
        frontClockwise,
        frontHalf,
        frontCounterclockwise,
        backClockwise,
        backHalf,
        backCounterclockwise
    };

    Algorithm(std::vector<enum Turn> turns);
    Algorithm(std::string stringNotation);

    std::string ToStringNotation();

    const std::vector<enum Turn> &turns();

  private:
    std::vector<enum Turn> turns_;
};

#endif
