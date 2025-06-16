#include <iostream>
#include <string>
#include <vector>

#include <fmt/core.h>

#include "cube.h"

using namespace cubot;

const std::string kFaceletToString[] = {"⬜️", "🟨", "🟥", "🟧", "🟩", "🟦"};

void net(const Cube &cube) {
    std::vector<std::string> fs;
    for (auto facelet : cube.ToFacelets())
        fs.push_back(kFaceletToString[static_cast<int>(facelet)]);

    fmt::print("         {} {} {}\n", fs[0], fs[1], fs[2]);
    fmt::print("         {} {} {}\n", fs[3], fs[4], fs[5]);
    fmt::print("         {} {} {}\n", fs[6], fs[7], fs[8]);
    fmt::print("{} {} {} {} {} {} {} {} {} {} {} {}\n", fs[27], fs[28], fs[29],
               fs[36], fs[37], fs[38], fs[18], fs[19], fs[20], fs[45], fs[46],
               fs[47]);
    fmt::print("{} {} {} {} {} {} {} {} {} {} {} {}\n", fs[30], fs[31], fs[32],
               fs[39], fs[40], fs[41], fs[21], fs[22], fs[23], fs[48], fs[49],
               fs[50]);
    fmt::print("{} {} {} {} {} {} {} {} {} {} {} {}\n", fs[33], fs[34], fs[35],
               fs[42], fs[43], fs[44], fs[24], fs[25], fs[26], fs[51], fs[52],
               fs[53]);
    fmt::print("         {} {} {}\n", fs[9], fs[10], fs[11]);
    fmt::print("         {} {} {}\n", fs[12], fs[13], fs[14]);
    fmt::print("         {} {} {}\n", fs[15], fs[16], fs[17]);
}

int main() {
    Cube cube;
    std::string algorithm;

    while (1) {
        net(cube);
        std::getline(std::cin, algorithm);
        cube.Execute(Algorithm(algorithm));
    }

    return 0;
}
