#include <math.h>
#include <stdio.h>

#include "image.h"

int colors_equal(struct Color color_a, struct Color color_b) {

    return color_a.red == color_b.red && color_a.green == color_b.green &&
           color_a.blue == color_b.blue;
}

int main() {
    struct Color colors[9];
    extract_face_colors("/Users/eamontracey/Downloads/pixel.jpg", colors);

    struct Color white = (struct Color){255, 255, 255};
    struct Color yellow = (struct Color){255, 255, 0};
    struct Color green = (struct Color){0, 255, 0};
    struct Color blue = (struct Color){100, 100, 255};
    struct Color red = (struct Color){255, 0, 0};
    struct Color orange = (struct Color){255, 165, 0};
    struct Color cube_colors[6] = {white, yellow, green, blue, red, orange};

    for (int i = 0; i < 9; ++i) {
        double closest_distance = INFINITY;
        struct Color closest_color;
        for (int j = 0; j < 6; ++j) {
            double distance = color_distance(colors[i], cube_colors[j]);
            if (distance < closest_distance) {
                closest_distance = distance;
                closest_color = cube_colors[j];
            }
        }
        //        fprintf(stdout, "(%d,%d,%d)\n", colors[i].red,
        //        colors[i].green, colors[i].blue);

        if (colors_equal(closest_color, white)) {
            fprintf(stdout, "WHITE\n");
        } else if (colors_equal(closest_color, yellow)) {
            fprintf(stdout, "YELLOW\n");
        } else if (colors_equal(closest_color, green)) {
            fprintf(stdout, "GREEN\n");
        } else if (colors_equal(closest_color, blue)) {
            fprintf(stdout, "BLUE\n");
        } else if (colors_equal(closest_color, red)) {
            fprintf(stdout, "RED\n");
        } else if (colors_equal(closest_color, orange)) {
            fprintf(stdout, "ORANGE\n");
        } else {
            fprintf(stdout, "UNKNOWN");
        }
    }

    return 0;
}
