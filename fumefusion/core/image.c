#include "image.h"


PPMPixel* PPMPixel_create(unsigned char r, unsigned char g, unsigned char b) {
    PPMPixel* pixel = (PPMPixel*)malloc(sizeof(PPMPixel));
    if (!pixel) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    pixel->r = r;
    pixel->g = g;
    pixel->b = b;
    return pixel;
}

void PPMPixel_free(PPMPixel* pixel) {
    if (pixel) {
        free(pixel);
    }
}




PPMImage* PPMImage_create(int width, int height) {
    PPMImage* img = (PPMImage*)malloc(sizeof(PPMImage));
    if (!img) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    img->width = width;
    img->height = height;
    img->data = (unsigned char*)malloc(width * height * 3);
    if (!img->data) {
        fprintf(stderr, "Memory allocation failed\n");
        free(img);
        return NULL;
    }

    return img;
}


PPMImage* PPMImage_read(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Unable to open file %s\n", filename);
        return NULL;
    }

    char format[3];
    if (!fgets(format, sizeof(format), file)) {
        fprintf(stderr, "Invalid PPM file format\n");
        fclose(file);
        return NULL;
    }

    if (format[0] != 'P' || format[1] != '6') {
        fprintf(stderr, "Unsupported PPM format (must be P6)\n");
        fclose(file);
        return NULL;
    }

    PPMImage* img = (PPMImage*)malloc(sizeof(PPMImage));
    if (!img) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    if (fscanf(file, "%d %d", &img->width, &img->height) != 2) {
        fprintf(stderr, "Invalid image size\n");
        free(img);
        fclose(file);
        return NULL;
    }

    int max_val;
    if (fscanf(file, "%d", &max_val) != 1 || max_val != 255) {
        fprintf(stderr, "Invalid max value\n");
        free(img);
        fclose(file);
        return NULL;
    }

    fgetc(file); // Skip the newline character after max_val

    size_t img_size = img->width * img->height * 3;
    img->data = (unsigned char*)malloc(img_size);
    if (!img->data) {
        fprintf(stderr, "Memory allocation failed\n");
        free(img);
        fclose(file);
        return NULL;
    }

    if (fread(img->data, 1, img_size, file) != img_size) {
        fprintf(stderr, "Error reading image data\n");
        free(img->data);
        free(img);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return img;
}

int PPMImage_write(const char* filename, const PPMImage* img) {
    if (!img || !img->data) return -1;

    FILE* file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Unable to open file %s\n", filename);
        return -1;
    }

    fprintf(file, "P6\n%d %d\n255\n", img->width, img->height);
    size_t img_size = img->width * img->height * 3;
    if (fwrite(img->data, 1, img_size, file) != img_size) {
        fprintf(stderr, "Error writing image data\n");
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}

void PPMImage_free(PPMImage* img) {
    if (img) {
        free(img->data);
        free(img);
    }
}
