#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "hitable.h"

class sphere: public hitable {
public:
    sphere();
    sphere(vec3 center, float radius): center(center), radius(radius) {};
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    vec3 center;
    float radius;
};


bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b -  a * c;
    if(discriminant > 0) {
        float temp = (-b - sqrt(b * b - a * c))/ a;
        if(temp < t_max && temp > t_min){
            rec.t = temp;
            rec.position = r.point_at_parameter(rec.t);
            rec.normal = (rec.position - center) / radius;
            return true;
        }
        temp = (-b + sqrt(b * b - a * c))/ a;
        if(temp < t_max && temp > t_min){
            rec.t = temp;
            rec.position = r.point_at_parameter(rec.t);
            rec.normal = (rec.position - center) / radius;
            return true;
        }
    }
    return false;
}

#endif //RAYTRACER_SPHERE_H
