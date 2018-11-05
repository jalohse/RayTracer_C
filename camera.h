#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "ray.h"

vec3 random_in_unit_disk() {
    vec3 p;
    do {
        p = 2.0 * vec3(drand48(), drand48(), 0) - vec3(1, 1, 0);
    } while (dot(p, p) >= 1.0);
    return p;
}

class camera {
public:
    camera(vec3 look_from, vec3 look_at, vec3 view_up, float field_of_view, float aspect_ratio,
           float aperature, float focus_distance, float t0, float t1) {
        time0 = t0;
        time1 = t1;
        lens_radius = aperature / 2;
        float theta = field_of_view * M_PI / 180;
        float half_height = tan(theta / 2);
        float half_width = aspect_ratio * half_height;
        origin = look_from;
        w = unit_vector(look_from - look_at);
        u = unit_vector(cross(view_up, w));
        v = cross(w, u);
        lower_left_corner = origin - half_width * focus_distance * u - half_height * focus_distance * v
                - focus_distance * w;
        horizontal = 2 * half_width * focus_distance  * u;
        vertical = 2 * half_height * focus_distance * v;
    }
    ray get_ray(float s, float t) {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
        float time = time0 + drand48() * (time1 - time0);
        return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset, time);
    }
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    float time0, time1;
    float lens_radius;
};

#endif //RAYTRACER_CAMERA_H
