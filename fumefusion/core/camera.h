#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct PCamera_s PCamera;
struct PCamera_s {
    Vec3 *position;
    Vec3 *target;
} ;
// Function to initialize a camera
PCamera* PCamera_create(Vec3 *position, Vec3 *target);

// Function to free the camera memory
void PCamera_free(PCamera* camera);

// Function to project a 3D point onto a 2D plane
Vec2* PCamera_project(const PCamera* camera, const Vec3* point, float zoom, float aspectRatio);
#endif // CAMERA_H
