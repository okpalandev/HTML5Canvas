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
    // Calculate the direction from camera position to the projected point
    Vec3* direction = Vec3_sub(camera->direction, camera->position);
    
    // Calculate the projected point based on UV coordinates, zoom, and aspect ratio
    Vec3* projected = Vec3_create(
        camera->position->x + direction->x * uv->x * zoom * aspectRatio,
        camera->position->y + direction->y * uv->y * zoom,
        camera->position->z + direction->z * zoom
    );

    // Free the direction vector
    Vec3_free(direction);
    
    return projected;
}

