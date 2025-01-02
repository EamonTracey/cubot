#ifndef CUBE_H
#define CUBE_H

#include <stdint.h>

struct Cube {
    uint8_t edge_orientation[12];
    uint8_t edge_permutation[12];
    uint8_t corner_orientation[8];
    uint8_t corner_permutation[8];
};

int cubes_equal(struct Cube *cube_a, struct Cube *cube_b);

void apply_edge_orientation(struct Cube *cube, uint8_t edge_orientation[12]);
void apply_edge_permutation(struct Cube *cube, uint8_t edge_permutation[12]);
void apply_corner_orientation(struct Cube *cube, uint8_t corner_orientation[8]);
void apply_corner_permutation(struct Cube *cube, uint8_t corner_permutation[8]);
void apply_element(struct Cube *cube, struct Cube *element);

#endif
