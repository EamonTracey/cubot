#include <stdint.h>

#include "cube.h"

void apply_edge_orientation(struct Cube *cube, uint8_t edge_orientation[12]) {
    for (int i = 0; i < 12; ++i) {
        cube->edge_orientation[i] += edge_orientation[i];
        cube->edge_orientation[i] %= 2;
    }
}

void apply_edge_permutation(struct Cube *cube, uint8_t edge_permutation[12]) {
    uint8_t new_edge_permutation[12];
    for (int i = 0; i < 12; ++i)
        new_edge_permutation[i] = cube->edge_permutation[edge_permutation[i]];
    for (int i = 0; i < 12; ++i)
        cube->edge_permutation[i] = new_edge_permutation[i];
}

void apply_corner_orientation(struct Cube *cube,
                              uint8_t corner_orientation[8]) {
    for (int i = 0; i < 8; ++i) {
        cube->corner_orientation[i] += corner_orientation[i];
        cube->corner_orientation[i] %= 3;
    }
}

void apply_corner_permutation(struct Cube *cube,
                              uint8_t corner_permutation[8]) {
    uint8_t new_corner_permutation[8];
    for (int i = 0; i < 8; ++i)
        new_corner_permutation[i] =
            cube->corner_permutation[corner_permutation[i]];
    for (int i = 0; i < 8; ++i)
        cube->corner_permutation[i] = new_corner_permutation[i];
}

void apply_element(struct Cube *cube, struct Cube *element) {
    apply_edge_orientation(cube, element->edge_orientation);
    apply_edge_permutation(cube, element->edge_permutation);
    apply_corner_orientation(cube, element->corner_orientation);
    apply_corner_permutation(cube, element->corner_permutation);
}
