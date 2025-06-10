#include <fmt/core.h>

#include "algorithm.h"
#include "cube.h"

int main() {
    Cube cube;
    Algorithm tperm("R U R' U' R' F R2 U' R' U' R U R' F'");

    cube.Execute(tperm);
    fmt::print("solved? {}\n", cube == Cube::kSolvedCube);
    cube.Execute(tperm);
    fmt::print("solved? {}\n", cube == Cube::kSolvedCube);

    Cube c1;
    Cube c2;

    c1.Execute(tperm);
    c2.Apply(c1);

    fmt::print("c1==c2? {}\n", c1 == c2);

    c2.Apply(c2);
    fmt::print("solved {}\n", Cube::kSolvedCube == c2);

    return 0;
}
