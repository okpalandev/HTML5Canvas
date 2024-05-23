#include "camera.h"
#include <stdlib.h>

PCamera* PCamera_create(Vec3* position, Vec3* direction) {
    PCamera* camera = (PCamera*)malloc(sizeof(PCamera));
    if (camera) {
        camera->position = position;
        camera->direction = direction;
    }
    return camera;
}

void PCamera_free(PCamera* camera) {
    if (camera) {
        free(camera->position);
        free(camera->direction);
        free(camera);
    }
}

Vec3* PCamera_project(PCamera* camera, Vec2* uv, float zoom, float aspectRatio) {
    Vec3* projected = Vec3_create(uv->x * zoom * aspectRatio, uv->y * zoom, 1.0);
    return projected;
}
