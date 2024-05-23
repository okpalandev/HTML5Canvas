#include "core/vector.h"
#include "core/camera.h"
#include "core/image.h"
#include "core/ray.h"

#include <stdio.h>
#include <stdlib.h>

const int LAMINAR_FLOW_LOWER_LIMIT = 0;
const int LAMINAR_FLOW_UPPER_LIMIT = 2300;
const int TRANSITION_FLOW_UPPER_LIMIT = 3500;


int main() {
   
  Vec3* position = Vec3_create(0, 0, -50);
  Vec3* target = Vec3_create(0, 0, 1000);
  PCamera* camera = PCamera_create(position, target);
  PPMImage* image = PPMImage_create(800, 600);
  Ray* ray = Ray_create(position, target);
  PPMPixel* pixel = PPMPixel_create(0, 0, 0);
  PPMPixel_set(pixel, 255, 255, 255);
  PPMImage_write("output.ppm", image);
  Ray_free(ray);
  PPMPixel_free(pixel);
  PCamera_free(camera);
  PPMImage_free(image); // Fixed function name
  Vec3_free(position);
  Vec3_free(target);
  return 0;
}
