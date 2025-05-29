#include <stdint.h>
#include <stdlib.h>

#include "cube.h"

// oriented = 0
// flipped = 1
uint8_t flip_edge[2][2] = {{0, 1}, {1, 0}};

// oriented = 0
// clockwise = 1
// counterclockwise = 2
uint8_t twist_corner[3][3] = {{0, 1, 2}, {1, 2, 0}, {2, 0, 1}};

const struct Cube SOLVED_CUBE = (struct Cube){
    {{0, 0},
     {0, 1},
     {0, 2},
     {0, 3},
     {0, 4},
     {0, 5},
     {0, 6},
     {0, 7},
     {0, 8},
     {0, 9},
     {0, 10},
     {0, 11}},
    {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}}};

struct Cube *init_cube_solved() {
    struct Cube *solved_cube = (struct Cube *)malloc(sizeof(struct Cube));
    *solved_cube = SOLVED_CUBE;
    return solved_cube;
}

struct Cube *init_cube_copy(const struct Cube *cube) {
    struct Cube *copy_cube = (struct Cube *)malloc(sizeof(struct Cube));
    *copy_cube = *cube;
    return copy_cube;
}

int cubes_equal(const struct Cube *cube_a, const struct Cube *cube_b) {
    for (int i = 0; i < 12; ++i) {
        if (cube_a->edges[i].orientation != cube_b->edges[i].orientation ||
            cube_a->edges[i].permutation != cube_b->edges[i].permutation)
            return 0;
        if (i < 8 &&
            (cube_a->corners[i].orientation != cube_b->corners[i].orientation ||
             cube_a->corners[i].permutation != cube_b->corners[i].permutation))
            return 0;
    }

    return 1;
}

void apply_cube(struct Cube *cube, const struct Cube *cube_to_apply) {
    struct Cube new_cube;

    for (int i = 0; i < 12; ++i) {
        new_cube.edges[i] = cube->edges[cube_to_apply->edges[i].permutation];
        new_cube.edges[i].orientation =
            flip_edge[cube_to_apply->edges[i].orientation]
                     [new_cube.edges[i].orientation];

        if (i < 8) {
            new_cube.corners[i] =
                cube->corners[cube_to_apply->corners[i].permutation];
            new_cube.corners[i].orientation =
                twist_corner[cube_to_apply->corners[i].orientation]
                            [new_cube.corners[i].orientation];
        }
    }

    *cube = new_cube;
}

void turn_up_cw(struct Cube *cube) {

}

void turn_up_half(struct Cube *cube) {

}

void turn_up_ccw(struct Cube *cube) {

}
