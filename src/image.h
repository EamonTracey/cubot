#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

int extract_face_colors(char *path, struct Color colors[9]);
double color_distance(struct Color color_a, struct Color color_b);

#endif
