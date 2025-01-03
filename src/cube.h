#ifndef CUBE_H
#define CUBE_H

#include <stdint.h>

struct Edge {
    uint8_t orientation;
    uint8_t permutation;
};

struct Corner {
    uint8_t orientation;
    uint8_t permutation;
};

struct Cube {
    struct Edge edges[12];
    struct Corner corners[8];
};

extern struct Cube SOLVED_CUBE;

struct Cube *init_cube_solved();
struct Cube *init_cube_copy(struct Cube *cube);
int cubes_equal(struct Cube *cube_a, struct Cube *cube_b);
void apply_cube(struct Cube *cube, struct Cube *cube_to_apply);

#endif
