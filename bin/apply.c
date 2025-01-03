#include <stdio.h>

#include "cube.h"

int main() {
    struct Cube cube = SOLVED_CUBE;

    struct Cube cube_to_apply = (struct Cube){
        {{0, 0},
         {0, 7},
         {0, 2},
         {0, 3},
         {0, 4},
         {0, 5},
         {0, 6},
         {1, 1},
         {0, 8},
         {0, 9},
         {0, 10},
         {0, 11}},
        {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}}};

    ////////////////////////////////////////////////////////////////////

    for (int i = 0; i < 12; ++i)
        printf("%d %d, ", cube.edges[i].orientation, cube.edges[i].permutation);
    printf("\n");

    apply_cube(&cube, &cube_to_apply);

    for (int i = 0; i < 12; ++i)
        printf("%d %d, ", cube.edges[i].orientation, cube.edges[i].permutation);
    printf("\n");

    ////////////////////////////////////////////////////////////////////

    return 0;
}
