#ifndef CUBOT_BRIDGE_H
#define CUBOT_BRIDGE_H

#include <array>
#include <string>

#include "cube.h"

typedef std::array<enum cubot::Cube::Facelet, 54> FaceletsArray;

bool IsValidAlgorithm(std::string stringNotation);

#endif
