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

extern const struct Cube SOLVED_CUBE;

struct Cube *init_cube_solved();
struct Cube *init_cube_copy(const struct Cube *cube);
int cubes_equal(const struct Cube *cube_a, const struct Cube *cube_b);

void apply_cube(const struct Cube *cube, const struct Cube *cube_to_apply);
void turn_up_cw(struct Cube *cube);
void turn_up_half(struct Cube *cube);
void turn_up_ccw(struct Cube *cube);

#endif
