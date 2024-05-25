#include "core/vector.h"
#include "core/camera.h"
#include "core/image.h"
#include "core/ray.h"
#include "core/sphere.h"
#include "core/material.h"
#include "core/hit.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

const int LAMINAR_FLOW_LOWER_LIMIT = 0;
const double LAMINAR_FLOW_UPPER_LIMIT = 2300; 
const double TRANSITION_FLOW_UPPER_LIMIT = 3500;

// Function to shoot a ray and check for intersection with the sphere
Hit* shoot_ray(PCamera* camera, Vec2* uv, Sphere* sphere) {
    Vec3 *point = PCamera_project(camera, uv, 1.0f, 1.0f); // Assume aspect ratio of 1:1
    Ray *ray = Ray_create(camera->position, Vec3_sub(point, camera->position));
    Hit *hit = Sphere_intersect(sphere, ray);
    Ray_free(ray);
    Vec3_free(point);
    return hit;
}

int main() {

    // Create the camera    
    Vec3 *camera_position = Vec3_create(0, 0, 10); // Move the camera further from the scene
    Vec3 *camera_direction = Vec3_create(0, 0, -1);
    PCamera *camera = PCamera_create(camera_position, camera_direction);

    // Create the sphere
    Vec3 *sphere_center = Vec3_create(0, 0, 0);  // Keep the sphere centered
    double sphere_radius = 2.0; // Adjust the sphere radius
    Material *sphere_material = Material_create(Vec3_create(0.9, 0.2, 0.2), 0.5, 1, 0.8);  // Adjust material color
    Sphere *sphere = Sphere_create(sphere_center, sphere_radius, sphere_material);

    // Create the image
    PPMImage *image = PPMImage_create(512, 512);
    if (!image || !image->data) {
        fprintf(stderr, "Error: Could not create image.\n");
        return 1;
    };


    // Render the image
    srand(time(NULL)); // Seed the random number generator
    int num_rays = 100;
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            double intensity_sum = 0.0;
            for (int i = 0; i < num_rays; i++) {
                // Generate random jitter within the pixel
                double jitter_x = ((double)rand() / RAND_MAX) - 0.5; // Range [-0.5, 0.5]
                double jitter_y = ((double)rand() / RAND_MAX) - 0.5; // Range [-0.5, 0.5]
                Vec2 uv = { ((double)x + jitter_x) / image->width, ((double)y + jitter_y) / image->height };
                Hit *hit = shoot_ray(camera, &uv, sphere);
                if (hit && hit->t > 0) {
                    Vec3 *normal = Vec3_normalize(Vec3_sub(hit->position, sphere->center));
                    double intensity = fabs(Vec3_dot(normal, camera_direction)); // Use absolute value for intensity
                    intensity = intensity > 1 ? 1 : intensity;  // Clamp intensity to [0, 1]
                    intensity_sum += intensity;
                }
                Hit_free(hit);
            }
            double average_intensity = intensity_sum / num_rays;
            PPMImage_setPixel(image, x, y, average_intensity, average_intensity, average_intensity);
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
