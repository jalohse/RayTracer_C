#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "hitable.h"

class material {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1,1,1);
    } while (p.squared_length() >= 1.0);
    return p;
}

vec3 reflect(const vec3& vector, const vec3& normal) {
    return vector - 2 * dot(vector, normal) * normal;
}

bool refract(const vec3& vector, const vec3& normal, float ni_over_nt, vec3& refracted) {
    vec3 uv = unit_vector(vector);
    float dt = dot(uv, normal);
    float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
    if (discriminant > 0) {
        refracted = ni_over_nt * (uv - normal * dt) - normal * sqrt(discriminant);
        return true;
    }
    return false;
}

float schlick(float cosine, float reflection_index) {
    float r0 = (1 - reflection_index) / (1 + reflection_index);
    r0 = r0 * r0;
    return  r0 + (1 - r0) * pow((1 - cosine), 5);
}

class dielectric: public material {
public:
    dielectric(float ri): reflection_index(ri) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 outward_normal;
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        float ni_over_nt;
        attenuation = vec3(1.0, 1.0, 1.0);
        vec3 refracted;
        float reflect_probability;
        float cosine;
        if(dot(r_in.direction(), rec.normal) > 0) {
            outward_normal = -rec.normal;
            ni_over_nt = reflection_index;
            cosine = reflection_index * dot(r_in.direction(), rec.normal) / r_in.direction().length();
        } else {
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / reflection_index;
            cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        if(refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
            reflect_probability = schlick(cosine, reflection_index);
        } else {
            scattered = ray(rec.position, reflected);
            reflect_probability = 1.0;
        }
        if(drand48() < reflect_probability) {
            scattered = ray(rec.position, reflected);
        } else {
            scattered = ray(rec.position, refracted);
        }
        return true;
    }
    float reflection_index;
};

class metal : public material {
public:
    metal(const vec3& a, float f): albedo(a) { if (f< 1) fuzz = f; else fuzz =1;};
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.position, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return dot(scattered.direction(), rec.normal) > 0;
    }
    vec3 albedo;
    float fuzz;
};

class lambertian: public material {
public:
    lambertian(const vec3& a): albedo(a) {};
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 target = rec.position + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.position, target - rec.position, r_in.time());
        attenuation = albedo;
        return true;
    }
    vec3 albedo;
};



#endif //RAYTRACER_MATERIAL_H
