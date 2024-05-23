#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct PPMPixel PPMPixel;
struct PPMPixel {
    unsigned int r, g, b;
};

PPMPixel* PPMPixel_create(unsigned int r, unsigned int g, unsigned int b);
void PPMPixel_set(PPMPixel* pixel, unsigned int r, unsigned int g, unsigned int b);
void PPMPixel_free(PPMPixel* pixel);

typedef struct PPMImage PPMImage;
struct PPMImage {
    int width;
    int height;
    PPMPixel* data; 
};

PPMImage* PPMImage_create(unsigned int width, unsigned int height);
PPMImage* PPMImage_read(const char* filename);
int PPMImage_write(const char* filename, const PPMImage* img);
void PPMImage_free(PPMImage* img);
void PPMImage_setPixel(PPMImage* img, int x, int y, unsigned int r, unsigned int g, unsigned int b);

#endif // IMAGE_H
