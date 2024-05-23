#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#define core

#include "vector.h"
#include "ray.h"

typedef struct {
  Vec3* center;
  double radius;
} Sphere;


Sphere* Sphere_create(Vec3* center, double radius);
void Sphere_free(Sphere* sphere);


#endif // SPHERE_H