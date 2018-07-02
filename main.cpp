#include <iostream>
#include <fstream>
#include "sphere.h"
#include "hitable_list.h"
using namespace std;

vec3 getColor(const ray &ray, hitable *world) {
    hit_record record;
    if(world->hit(ray, 0.0, MAXFLOAT, record)){
        return 0.5 * vec3(record.normal.x() + 1, record.normal.y() +1, record.normal.z() + 1);
    }
    vec3 unit_direction = unit_vector(ray.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main() {
    int nx = 200;
    int ny = 100;
    ofstream file;
    file.open("example.ppm");
    file << "P3\n" << nx << " " << ny << "\n255\n";
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);
    hitable *list[2];
    list[0] = new sphere(vec3(0,0,-1), 0.5);
    list[1] = new sphere(vec3(0, -100.5, -1), 100);
    hitable *world = new hitable_list(list, 2);
    for (int j = ny-1; j >= 0 ; j--) {
        for (int i = 0; i < nx; ++i) {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            ray r(origin, lower_left_corner + u * horizontal + v * vertical);
            vec3 color = getColor(r, world);
            int ir = int(255.99 * color[0]);
            int ig = int(255.99 * color[1]);
            int ib = int(255.99 * color[2]);
            file << ir << " " << ig << " " << ib << "\n";
        }
    }
    file.close();
    return 0;
}