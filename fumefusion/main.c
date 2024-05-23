#include "core/vector.h"
#include "core/camera.h"
#include "core/image.h"
#include "core/ray.h"
#include "core/sphere.h"
#include "core/material.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h> // Ensure math.h is included for pow function

const int LAMINAR_FLOW_LOWER_LIMIT = 0;
const double LAMINAR_FLOW_UPPER_LIMIT = 2300;
const double TRANSITION_FLOW_UPPER_LIMIT = 3500;

int main() {
    // Create the camera    
    Vec3 *camera_position = Vec3_create(0, 0, 0);
    Vec3 *camera_direction = Vec3_create(0, 0, 1);
    PCamera *camera = PCamera_create(camera_position, camera_direction);

    // Create the sphere
    Vec3 *sphere_center = Vec3_create(0, 0, 3);
    double sphere_radius = 1.0;
    Material *sphere_material = Material_create(Vec3_create(0, 0, 0), 0.5, 1, 0.8);
    Sphere *sphere = Sphere_create(sphere_center, sphere_radius, sphere_material);

    // Create the image
    PPMImage *image = PPMImage_create(512, 512);
    if (!image || !image->data) {
        fprintf(stderr, "Error: Could not create image.\n");
        return 1;
    }

    // Render the image
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            Vec2 uv = { (double)x / image->width, (double)y / image->height };
            Vec3 *point = PCamera_project(camera, &uv, 1.0f, (float)image->width / image->height);
            Ray *ray = Ray_create(camera->position, Vec3_sub(point, camera->position));
            Hit *hit = Sphere_intersect(sphere, ray);
            if (hit && hit->t > 0) {
                Vec3 *normal = Vec3_normalize(Vec3_sub(hit->position, sphere->center));
                double intensity = Vec3_dot(normal, Vec3_create(0, 0, 1));
                intensity = intensity < 0 ? 0 : intensity;
                intensity = intensity > 1 ? 1 : intensity;
                intensity = pow(intensity, 1 / 2.2);
                PPMImage_setPixel(image, x, y, intensity, intensity, intensity);
            }
            Hit_free(hit);
            Ray_free(ray);
            Vec3_free(point);
        }
    }

    // Save the image
    if (!PPMImage_write("output.ppm", image)) {
        fprintf(stderr, "Error: Could not write image to file.\n");
        return 1;
    }

    // Free the memory
    PCamera_free(camera);
    Sphere_free(sphere);
    PPMImage_free(image);

    return 0;
}
