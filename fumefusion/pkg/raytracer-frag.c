#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define M_PI 3.14159265358979323846

typedef struct {
    float x, y, z;
} Vec3;

Vec3 Vec3_create(float x, float y, float z) {
    Vec3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

Vec3 Vec3_add(Vec3 v1, Vec3 v2) {
    Vec3 result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    return result;
}

Vec3 Vec3_subtract(Vec3 v1, Vec3 v2) {
    Vec3 result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
    return result;
}

Vec3 Vec3_multiply(Vec3 v, float scalar) {
    Vec3 result;
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    result.z = v.z * scalar;
    return result;
}

Vec3 Vec3_multiply_componentwise(Vec3 v1, Vec3 v2) {
    Vec3 result;
    result.x = v1.x * v2.x;
    result.y = v1.y * v2.y;
    result.z = v1.z * v2.z;
    return result;
}

float Vec3_dot(Vec3 v1, Vec3 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Vec3_length2(Vec3 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

float Vec3_length(Vec3 v) {
    return sqrt(Vec3_length2(v));
}

Vec3 Vec3_normalize(Vec3 v) {
    float nor2 = Vec3_length2(v);
    if (nor2 > 0) {
        float invNor = 1 / sqrt(nor2);
        v.x *= invNor;
        v.y *= invNor;
        v.z *= invNor;
    }
    return v;
}

typedef struct {
    Vec3 center;
    float radius, radius2;
    Vec3 surfaceColor, emissionColor;
    float transparency, reflection;
} Sphere;

Sphere Sphere_create(Vec3 center, float radius, Vec3 surfaceColor, float reflection, float transparency, Vec3 emissionColor) {
    Sphere s;
    s.center = center;
    s.radius = radius;
    s.radius2 = radius * radius;
    s.surfaceColor = surfaceColor;
    s.emissionColor = emissionColor;
    s.transparency = transparency;
    s.reflection = reflection;
    return s;
}

int Sphere_intersect(Sphere s, Vec3 rayorig, Vec3 raydir, float* t0, float* t1) {
    Vec3 l = Vec3_subtract(s.center, rayorig);
    float tca = Vec3_dot(l, raydir);
    if (tca < 0) return 0;
    float d2 = Vec3_dot(l, l) - tca * tca;
    if (d2 > s.radius2) return 0;
    float thc = sqrt(s.radius2 - d2);
    *t0 = tca - thc;
    *t1 = tca + thc;
    return 1;
}

float mix(float a, float b, float mix) {
    return b * mix + a * (1 - mix);
}

Vec3 trace(Vec3 rayorig, Vec3 raydir, Sphere* spheres, int numSpheres, int depth) {
    float tnear = INFINITY;
    Sphere* sphere = NULL;
    for (int i = 0; i < numSpheres; ++i) {
        float t0 = INFINITY, t1 = INFINITY;
        if (Sphere_intersect(spheres[i], rayorig, raydir, &t0, &t1)) {
            if (t0 < 0) t0 = t1;
            if (t0 < tnear) {
                tnear = t0;
                sphere = &spheres[i];
            }
        }
    };

    if (!sphere) return Vec3_create(2, 2, 2);
    Vec3 surfaceColor = Vec3_create(0, 0, 0);
    Vec3 phit = Vec3_add(rayorig, Vec3_multiply(raydir, tnear));
    Vec3 nhit = Vec3_subtract(phit, sphere->center);
    nhit = Vec3_normalize(nhit);
    float bias = 1e-4;
    int inside = 0;
    if (Vec3_dot(raydir, nhit) > 0) {
        nhit = Vec3_multiply(nhit, -1);
        inside = 1;
    }
    
    if ((sphere->transparency > 0 || sphere->reflection > 0) && depth < 5) {
        float facingratio = -Vec3_dot(raydir, nhit);
        float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1);
        Vec3 refldir = Vec3_subtract(raydir, Vec3_multiply(nhit, 2 * Vec3_dot(raydir, nhit)));
        refldir = Vec3_normalize(refldir);
        Vec3 reflection = trace(Vec3_add(phit, Vec3_multiply(nhit, bias)), refldir, spheres, numSpheres, depth + 1);
        Vec3 refraction = Vec3_create(0, 0, 0);
        if (sphere->transparency) {
            float ior = 1.1, eta = (inside) ? ior : 1 / ior;
            float cosi = -Vec3_dot(nhit, raydir);
            float k = 1 - eta * eta * (1 - cosi * cosi);
            Vec3 refrdir = Vec3_add(Vec3_multiply(raydir, eta), Vec3_multiply(nhit, eta * cosi - sqrt(k)));
            refrdir = Vec3_normalize(refrdir);
            refraction = trace(Vec3_subtract(phit, Vec3_multiply(nhit, bias)), refrdir, spheres, numSpheres, depth + 1);
        }
        surfaceColor = Vec3_multiply_componentwise(Vec3_add(Vec3_multiply(reflection, fresneleffect), Vec3_multiply(refraction, (1 - fresneleffect) * sphere->transparency)), sphere->surfaceColor);
    } else {
        for (int i = 0; i < numSpheres; ++i) {
            if (spheres[i].emissionColor.x > 0) {
                Vec3 transmission = Vec3_create(1, 1, 1);
                Vec3 lightDirection = Vec3_subtract(spheres[i].center, phit);
                lightDirection = Vec3_normalize(lightDirection);
                for (int j = 0; j < numSpheres; ++j) {
                    if (i != j) {
                        float t0, t1;
                        if (Sphere_intersect(spheres[j], Vec3_add(phit, Vec3_multiply(nhit, bias)), lightDirection, &t0, &t1)) {
                            transmission = Vec3_create(0, 0, 0);
                            break;
                        }
                    }
                }
                surfaceColor = Vec3_add(surfaceColor, Vec3_multiply_componentwise(Vec3_multiply_componentwise(sphere->surfaceColor, transmission), Vec3_create(fmax(0, Vec3_dot(nhit, lightDirection)), fmax(0, Vec3_dot(nhit, lightDirection)), fmax(0, Vec3_dot(nhit, lightDirection)))));
            }
        }
    }
    return Vec3_add(surfaceColor, sphere->emissionColor);
}


void render(Sphere* spheres, int numSpheres) {
    unsigned int width = 640, height = 480;
    Vec3* image = (Vec3*)malloc(width * height * sizeof(Vec3));
    Vec3* pixel = image;
    float invWidth = 1 / (float)width;
    float invHeight = 1 / (float)height;
    float fov = 30;
    float aspectratio = width / (float)height;
    float angle = tan(M_PI * 0.5 * fov / 180.);
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x, ++pixel) {
            float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio;
            float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
            Vec3 raydir = Vec3_create(xx, yy, -1);
            raydir = Vec3_normalize(raydir);
            *pixel = trace(Vec3_create(0, 0, 0), raydir, spheres, numSpheres, 0);
        }
    }
    FILE* file = fopen("smoke.ppm", "wb");
    fprintf(file, "P6\n%d %d\n255\n", width, height);
    for (unsigned int i = 0; i < width * height; ++i) {
        fprintf(file, "%c%c%c", (unsigned char)(fmin(1, image[i].x) * 255), (unsigned char)(fmin(1, image[i].y) * 255), (unsigned char)(fmin(1, image[i].z) * 255));
    }
    fclose(file);
    free(image);
}

int main(int argc, char** argv) {
    srand(13);
    int numSpheres = 6;
    Sphere* spheres = (Sphere*)malloc(numSpheres * sizeof(Sphere));
    spheres[0] = Sphere_create(Vec3_create(0.0, -10004, -20), 10000, Vec3_create(0.20, 0.20, 0.20), 0, 0.0, Vec3_create(0, 0, 0));
    spheres[1] = Sphere_create(Vec3_create(0.0, 0, -20), 4, Vec3_create(1.00, 0.32, 0.36), 1, 0.5, Vec3_create(0, 0, 0));
    spheres[2] = Sphere_create(Vec3_create(5.0, -1, -15), 2, Vec3_create(0.90, 0.76, 0.46), 1, 0.0, Vec3_create(0, 0, 0));
    spheres[3] = Sphere_create(Vec3_create(5.0, 0, -25), 3, Vec3_create(0.65, 0.77, 0.97), 1, 0.0, Vec3_create(0, 0, 0));
    spheres[4] = Sphere_create(Vec3_create(-5.5, 0, -15), 3, Vec3_create(0.90, 0.90, 0.90), 1, 0.0, Vec3_create(0, 0, 0));
    spheres[5] = Sphere_create(Vec3_create(0.0, 20, -30), 3, Vec3_create(0.00, 0.00, 0.00), 0, 0.0, Vec3_create(3, 3, 3));
    render(spheres, numSpheres);
    free(spheres);
    return 0;
}
