#include <iostream>
#include <fstream>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
using namespace std;

vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1,1,1);
    } while (p.squared_length() >= 1.0);
    return p;
}

vec3 getColor(const ray &r, hitable *world) {
    hit_record record;
    if(world->hit(r, 0.001, MAXFLOAT, record)){
        vec3 target = record.position + record.normal + random_in_unit_sphere();
        return 0.5 * getColor(ray(record.position, target - record.position), world);
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
    hitable *list[2];
    list[0] = new sphere(vec3(0,0,-1), 0.5);
    list[1] = new sphere(vec3(0, -100.5, -1), 100);
    hitable *world = new hitable_list(list, 2);
    camera camera;
    for (int j = ny-1; j >= 0 ; j--) {
        for (int i = 0; i < nx; ++i) {
            vec3 color(0,0,0);
            for(int s = 0; s < ns; s++) {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray camRay = camera.get_ray(u, v);
                color += getColor(camRay, world);
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