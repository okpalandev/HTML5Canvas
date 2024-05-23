   for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            Vec2 *uv = Vec2_create((double)x / image->width, (double)y / image->height);
            Vec3 *point = PCamera_project(camera, uv);
            Ray *ray = Ray_create(camera->position, Vec3_subtract(point, camera->position));
            Hit *hit = Sphere_intersect(sphere, ray);
            if (hit->t > 0) {
                Vec3 *normal = Vec3_normalize(Vec3_subtract(hit->point, sphere->center));
                double intensity = Vec3_dot(normal, Vec3_create(0, 0, 1));
                intensity = intensity < 0 ? 0 : intensity;
                intensity = intensity > 1 ? 1 : intensity;
                intensity = pow(intensity, 1 / 2.2);
                PPMImage_setPixel(image, x, y, Vec3_create(intensity, intensity, intensity));
            }
            Hit_free(hit);
            Ray_free(ray);
            Vec3_free(point);
            Vec2_free(uv);
        }
    }