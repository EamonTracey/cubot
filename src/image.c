#include <math.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jpeglib.h>

#include "image.h"

struct cubot_jpeg_error_mgr {
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};

void cubot_jpeg_error_exit(j_common_ptr cinfo) {
    struct cubot_jpeg_error_mgr *err =
        (struct cubot_jpeg_error_mgr *)cinfo->err;
    longjmp(err->setjmp_buffer, 1);
}

int extract_face_colors(char *path, struct Color colors[9]) {
    FILE *image_file;
    struct jpeg_decompress_struct cinfo;
    struct cubot_jpeg_error_mgr jerr;
    JSAMPROW buffer = NULL;
    uint64_t sums[27];
    uint32_t size;
    uint32_t facelet_size;
    uint32_t center_size;
    uint32_t center_area;
    uint32_t center_start;
    uint32_t center_end;

    image_file = fopen(path, "rb");
    if (image_file == NULL)
        return 1;

    // Execution returns to this setjmp upon a fatal libjpeg error.
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = cubot_jpeg_error_exit;
    if (setjmp(jerr.setjmp_buffer)) {
        jpeg_destroy_decompress(&cinfo);
        fclose(image_file);
        if (buffer != NULL)
            free(buffer);
        return 1;
    }

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, image_file);
    jpeg_read_header(&cinfo, TRUE);
    cinfo.out_color_space = JCS_RGB;

    // Enforce a square image with at least 3x3 resolution.
    if (cinfo.image_width != cinfo.image_height || cinfo.image_width < 3) {
        jpeg_destroy_decompress(&cinfo);
        fclose(image_file);
        return 1;
    }

    size = cinfo.image_width;
    facelet_size = size / 3;
    center_size = facelet_size / 3 ? facelet_size / 3 : 1;
    center_area = center_size * center_size;
    center_start = facelet_size / 3;
    center_end = facelet_size - center_start;

    // Compute the RGB sum of each facelet. To account for image imperfection,
    // we strictly use the pixels in the center of each facelet. The center of a
    // facelet is the central square with a ninth of the area of the facelet.
    memset(sums, 0, sizeof(sums));
    buffer = (JSAMPROW)malloc(sizeof(JSAMPLE) * size * 3);
    jpeg_start_decompress(&cinfo);
    for (uint32_t i = 0; i < size; ++i) {
        uint32_t facelet_row = i / facelet_size;

        jpeg_read_scanlines(&cinfo, &buffer, 1);
        for (uint32_t j = 0; j < size; ++j) {
            uint32_t facelet_column = (uint32_t)(j / facelet_size);

            // Skip pixels outside the center region of the facelet.
            uint32_t facelet_i = (uint32_t)(i % facelet_size);
            uint32_t facelet_j = (uint32_t)(j % facelet_size);
            if (facelet_i < center_start || facelet_i > center_end ||
                facelet_j < center_start || facelet_j > center_end) {
                continue;
            }

            uint32_t facelet = (uint32_t)(facelet_row * 3 + facelet_column);
            sums[facelet * 3] += buffer[j * 3];
            sums[facelet * 3 + 1] += buffer[j * 3 + 1];
            sums[facelet * 3 + 2] += buffer[j * 3 + 2];
        }
    }

    // Compute the average RGB values of each facelet.
    for (int i = 0; i < 9; ++i) {
        colors[i].red = (uint8_t)(sums[i * 3] / center_area);
        colors[i].green = (uint8_t)(sums[i * 3 + 1] / center_area);
        colors[i].blue = (uint8_t)(sums[i * 3 + 2] / center_area);
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(image_file);
    free(buffer);

    return 0;
}

double color_distance(struct Color color_a, struct Color color_b) {
    return sqrt(pow((double)color_a.red - (double)color_b.red, 2.0) +
                pow((double)color_a.green - (double)color_b.green, 2.0) +
                pow((double)color_a.blue - (double)color_b.blue, 2.0));
}
