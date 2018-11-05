
#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "vec3.h"

class ray {
public:
    ray() {}
    ray(const vec3 &origin, const vec3 &direction, float time = 0.0) {
            originVector = origin;
            directionVector = direction;
            _time = time;
    }
    vec3 origin() const { return originVector; }
    vec3 direction() const { return directionVector; }
    float time() const { return _time; }
    vec3 point_at_parameter(float t) const {
        return originVector + t * directionVector; }

    vec3 originVector;
    vec3 directionVector;
    float _time;
};


#endif //RAYTRACER_RAY_H
