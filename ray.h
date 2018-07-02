
#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "vec3.h"

class ray {
public:
    ray() {}
    ray(const vec3 &origin, const vec3 &direction) { originVector = origin; directionVector = direction;}
    vec3 origin() const { return originVector; }
    vec3 direction() const { return directionVector; }
    vec3 point_at_parameter(float t) const {
        return originVector + t * directionVector; }

    vec3 originVector;
    vec3 directionVector;
};


#endif //RAYTRACER_RAY_H
