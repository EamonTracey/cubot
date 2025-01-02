#include <stdio.h>

#include "image.h"

int main(int argc, char *argv[]) {
    struct Color colors[9];

    if (argc != 2) {
        fprintf(stderr, "ERROR: Must specify one input jpg file.\n");
        return 1;
    }

    if (extract_face_colors(argv[1], colors)) {
        fprintf(stderr, "ERROR: Something went wrong trying to parse %s.\n",
                argv[1]);
        return 1;
    }

    for (int i = 0; i < 9; ++i) {
        printf("(%d, %d, %d)\n", colors[i].red, colors[i].green,
               colors[i].blue);
    }

    return 0;
}
