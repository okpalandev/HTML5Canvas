#include <stdio.h>
#include <stdlib.h>
#include "image.h"

PPMPixel* PPMPixel_create(unsigned int r, unsigned int g, unsigned int b) {
    PPMPixel* pixel = (PPMPixel*)malloc(sizeof(PPMPixel));
    if (!pixel) {
        return NULL; // Handle memory allocation failure
    }
    pixel->r = r;
    pixel->g = g;
    pixel->b = b;
    return pixel;
}

void PPMPixel_set(PPMPixel* pixel, unsigned int r, unsigned int g, unsigned int b) {
    if (pixel) {
        pixel->r = r;
        pixel->g = g;
        pixel->b = b;
    }
}

void PPMPixel_free(PPMPixel* pixel) {
    if (pixel) {
        free(pixel);
    }
}

PPMImage* PPMImage_create(unsigned int width, unsigned int height) {
    PPMImage* img = (PPMImage*)malloc(sizeof(PPMImage));
    if (!img) {
        return NULL; // Handle memory allocation failure
    }
    img->width = width;
    img->height = height;
    img->data = (PPMPixel*)malloc(width * height * sizeof(PPMPixel));
    if (!img->data) {
        free(img); // Clean up previously allocated memory
        return NULL; // Handle memory allocation failure
    }
    return img;
}

PPMImage* PPMImage_read(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        return NULL; // Handle file open failure
    }

    char format[256];
    if (!fgets(format, sizeof(format), file)) {
        fclose(file);
        return NULL; // Handle read failure
    }

    if (format[0] != 'P' || format[1] != '6') {
        fclose(file);
        return NULL; // Handle incorrect format
    }

    unsigned int width, height, max_val;
    fscanf(file, "%u %u %u", &width, &height, &max_val);
    fgetc(file); // Read past the newline after max_val

    PPMImage* img = PPMImage_create(width, height);
    if (!img) {
        fclose(file);
        return NULL; // Handle image creation failure
    }

    size_t img_size = width * height;
    if (fread(img->data, sizeof(PPMPixel), img_size, file) != img_size) {
        PPMImage_free(img);
        fclose(file);
        return NULL; // Handle read failure
    }

    fclose(file);
    return img;
}

int PPMImage_write(const char *filename, const PPMImage *img) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        return 0; // Handle file open failure
    }

    fprintf(file, "P6\n%u %u\n255\n", img->width, img->height);
    size_t img_size = img->width * img->height;
    if (fwrite(img->data, sizeof(PPMPixel), img_size, file) != img_size) {
        fclose(file);
        return 0; // Handle write failure
    }

    fclose(file);
    return 1;
}

void PPMImage_free(PPMImage *img) {
    if (img) {
        free(img->data);
        free(img);
    }
}

void PPMImage_setPixel(PPMImage* img, int x, int y, unsigned int r, unsigned int g, unsigned int b) {
    if (x < 0 || x >= img->width || y < 0 || y >= img->height) {
        return;
    }
    PPMPixel *pixel = &img->data[y * img->width + x];
    PPMPixel_set(pixel, r, g, b);
}
