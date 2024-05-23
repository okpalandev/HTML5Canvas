#ifndef PROCESSING_H
#define PROCESSING_H

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


// Define the PPMImage structure
typedef struct PPMImage PPMImage;
struct PPMImage {
    int width;
    int height;
    PPMPixel* data; // Array to hold pixel data (RGB values)
};

/* Function prototypes for PPM image processing */

// Function to create a new PPM image
PPMImage* PPMImage_create(unsigned int width, unsigned int height);
// Function to read a PPM image from a file
PPMImage* PPMImage_read(const char* filename);


// Function to write a PPM image to a file
int PPMImage_write(const char* filename, const PPMImage* img);

// Function to free the memory allocated for a PPM image
void PPMImage_free(PPMImage* img);


#endif // PROCESSING_H
