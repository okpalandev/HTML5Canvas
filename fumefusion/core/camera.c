#include "camera.h"

// Function to initialize a camera
PCamera* PCamera_create(Vec3 *position, Vec3 *target){
    PCamera* camera = (PCamera*)malloc(sizeof(PCamera));
    if (camera == NULL) {
        return NULL;
    }
    camera->position = position;
    camera->target = target;
    return camera;
}


// Function to free the camera memory
void PCamera_free(PCamera* camera) {
    if (camera != NULL) {
        free(camera);
    }
}

// Function to project a 3D point onto a 2D plane
Vec2* PCamera_project(const PCamera* camera, const Vec3* point, float zoom, float aspectRatio){
    // Calculate the vector from the camera position to this point
    Vec3 cameraToPoint = {
        point->x - camera->position->x,
        point->y - camera->position->y,
        point->z - camera->position->z
    };

    // Ensure the point is in front of the camera (cameraToPoint.z should not be zero)
    if (cameraToPoint.z == 0) {
        cameraToPoint.z = 0.0001f; // Avoid division by zero
    }

    // Calculate the projected coordinates using the aspect ratio
    float projectedX = (cameraToPoint.x / cameraToPoint.z) * zoom + (aspectRatio / 2);
    float projectedY = (cameraToPoint.y / cameraToPoint.z) * zoom + 0.5f; // Assuming aspect ratio is normalized

    Vec2 projectedPoint = { projectedX, projectedY };
    return Vec2_create(projectedPoint.x, projectedPoint.y);
}
