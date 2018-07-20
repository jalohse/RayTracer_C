#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "ray.h"

class camera {
public:
    camera(vec3 look_from, vec3 look_at, vec3 view_up, float field_of_view, float aspect_ratio) {
        vec3 u, v, w;
        float theta = field_of_view * M_PI / 180;
        float half_height = tan(theta / 2);
        float half_width = aspect_ratio * half_height;
        origin = look_from;
        w = unit_vector(look_from - look_at);
        u = unit_vector(cross(view_up, w));
        v = cross(w, u);
        lower_left_corner = origin - half_width * u - half_height * v - w;
        horizontal = 2 * half_width  * u;
        vertical = 2 * half_height * v;
    }
    ray get_ray(float u, float v) {
        return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
    }
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};

#endif //RAYTRACER_CAMERA_H
