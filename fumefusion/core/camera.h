#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"

typedef struct PCamera {
    Vec3 *position;
    Vec3 *direction;
} PCamera;

PCamera* PCamera_create(Vec3* position, Vec3* direction);
void PCamera_free(PCamera* camera);
Vec3* PCamera_project(PCamera* camera, Vec2* uv, float zoom, float aspectRatio);

#endif // CAMERA_H
