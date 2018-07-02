#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "hitable.h"

class material {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};


#endif //RAYTRACER_MATERIAL_H
