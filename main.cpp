#include <iostream>
#include <fstream>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"
using namespace std;

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
        scattered = ray(rec.position, target - rec.position);
        attenuation = albedo;
        return true;
    }
    vec3 albedo;
};

vec3 getColor(const ray &r, hitable *world, int depth) {
    hit_record record;
    if(world->hit(r, 0.001, MAXFLOAT, record)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && record.mat_ptr->scatter(r, record, attenuation, scattered)) {
            return attenuation * getColor(scattered, world, depth + 1);
        } else {
            return vec3(0, 0, 0);
        }
    }
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main() {
    int nx = 200;
    int ny = 100;
    int ns = 100;
    ofstream file;
    file.open("example.ppm");
    file << "P3\n" << nx << " " << ny << "\n255\n";
    hitable *list[5];
    list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
    list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0));
    list[3] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
    list[4] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
    hitable *world = new hitable_list(list, 5);
    camera camera(vec3(-2, 2, 1), vec3(0, 0, -1), vec3(0, 1, 0), 20, float(nx) / float(ny));
    for (int j = ny-1; j >= 0 ; j--) {
        for (int i = 0; i < nx; ++i) {
            vec3 color(0,0,0);
            for(int s = 0; s < ns; s++) {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray camRay = camera.get_ray(u, v);
                color += getColor(camRay, world, 0);
            }
            color /= float(ns);
            color = vec3(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));
            int ir = int(255.99 * color[0]);
            int ig = int(255.99 * color[1]);
            int ib = int(255.99 * color[2]);
            file << ir << " " << ig << " " << ib << "\n";
        }
    }
    file.close();
    return 0;
}