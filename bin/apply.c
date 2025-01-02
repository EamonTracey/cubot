#include <stdio.h>

#include "cube.h"

int main() {
    struct Cube solved = (struct Cube){{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                       {8, 5, 10, 0, 1, 2, 4, 6, 3, 7, 11, 9},
                                       {0, 0, 0, 0, 0, 0, 0, 0},
                                       {0, 1, 2, 3, 4, 5, 6, 7}};

    for (int i = 0; i < 11; ++i)
        printf("%d ", solved.edge_permutation[i]);
    printf("%d\n", solved.edge_permutation[11]);

    apply_edge_permutation(&solved,
                           (uint8_t[12]){2, 0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 11});

    for (int i = 0; i < 11; ++i)
        printf("%d ", solved.edge_permutation[i]);
    printf("%d\n", solved.edge_permutation[11]);

    return 0;
}
