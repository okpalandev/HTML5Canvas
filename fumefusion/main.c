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
  PPMImage* image = PPMImage_create(800, 600); // Fixed function name
  PPMPixel color = {255, 255, 255};

  for (int i = 0; i < image->width; i++) {
    for (int j = 0; j < image->height; j++) {
      Vec3* point = Vec3_create(i - image->width / 2, j - image->height / 2, 0);
      Vec2* projectedPoint = PCamera_project(camera, point, 1, 1);
      // PPMImage_setPixel(image, i, j, color); // Fixed function name
      Vec3_free(point);
      Vec2_free(projectedPoint);
    }
  } 

  PCamera_free(camera);
  PPMImage_write(image, "output.ppm"); // Fixed function name
  PPMImage_free(image); // Fixed function name
  Vec3_free(position);
  Vec3_free(target);
  return 0;
}
