#include "cubot_bridge.h"

#include <stdexcept>
#include <string>

#include "algorithm.h"

bool IsValidAlgorithm(std::string stringNotation) {
    try {
        (void)cubot::Algorithm(stringNotation);
    } catch (const std::invalid_argument &_) {
        return false;
    }
    return true;
}
