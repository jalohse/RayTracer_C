#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "hitable.h"

class material;

class moving_sphere: public hitable {
public:
    moving_sphere() {}
    moving_sphere(vec3 c0, vec3 c1, float t0, float t1, float r, material *m):
        center0(c0), center1(c1), time0(t0), time1(t1), radius(r), mat_ptr(m) {};
    virtual  bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const;
    vec3 center(float time) const;
    vec3 center0, center1;
    float time0, time1, radius;
    material *mat_ptr;

};

vec3 moving_sphere::center(float time) const {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool moving_sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center(r.time());
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b -  a * c;
    if(discriminant > 0) {
        float temp = (-b - sqrt(b * b - a * c))/ a;
        if(temp < t_max && temp > t_min){
            rec.t = temp;
            rec.position = r.point_at_parameter(rec.t);
            rec.normal = (rec.position - center(r.time())) / radius;
            rec.mat_ptr = this->mat_ptr;
            return true;
        }
        temp = (-b + sqrt(b * b - a * c))/ a;
        if(temp < t_max && temp > t_min){
            rec.t = temp;
            rec.position = r.point_at_parameter(rec.t);
            rec.normal = (rec.position - center(r.time())) / radius;
            rec.mat_ptr = this->mat_ptr;
            return true;
        }
    }
    return false;
}


class sphere: public hitable {
public:
    sphere();
    sphere(vec3 center, float radius, material *mat): center(center), radius(radius), mat_ptr(mat) {};
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    vec3 center;
    float radius;
    material *mat_ptr;
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
            rec.mat_ptr = this->mat_ptr;
            return true;
        }
        temp = (-b + sqrt(b * b - a * c))/ a;
        if(temp < t_max && temp > t_min){
            rec.t = temp;
            rec.position = r.point_at_parameter(rec.t);
            rec.normal = (rec.position - center) / radius;
            rec.mat_ptr = this->mat_ptr;
            return true;
        }
    }
    return false;
}



#endif //RAYTRACER_SPHERE_H
