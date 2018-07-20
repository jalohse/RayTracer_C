#include <iostream>
#include <fstream>
#include <thread>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"
using namespace std;

int nx = 600;
int ny = 300;
int ns = 100;

class PixelIterator {
public:
    PixelIterator () { Init(); };
    void Init() { ix = 0; };
    bool GetPixel(int &x, int &y) {
        int i = ix++;
        if( i > nx * ny) {
            return false;
        }
        y = i / nx;
        x = i - y * nx;
        return true;
    };
private:
    atomic<int> ix;
};

PixelIterator pixel_iterator;

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

hitable *random_scene() {
    int n = 500;
    hitable **list = new hitable*[n+1];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
    int i = 1;
    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            float choose_mat = drand48();
            vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
            if((center - vec3(4, 0.2, 0)).length() > 0.9) {
                if(choose_mat < 0.8) { //diffuse material
                    list[i++] = new sphere(center, 0.2, new lambertian(
                            vec3(drand48() * drand48(), drand48() * drand48(), drand48() * drand48())));
                } else if (choose_mat < 0.95) { // metal material
                    list[i++] = new sphere(center, 0.2,
                                           new metal(vec3(0.5 * (1 + drand48()), 0.5 * (1 + drand48()),
                                                          0.5 * (1 + drand48())), 0.5 * (1 + drand48())));
                } else { //glass  material
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }
    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
    return new hitable_list(list, i);
}

void RenderPixels() {
    int x, y;
    while(pixel_iterator.GetPixel(x,y)) {

    }
}

void BeginRender() {
    int n = thread::hardware_concurrency();
    if(n <= 0) {
        n = 1;
    }
    pixel_iterator.Init();
    for (int i = 0; i < n; ++i) {
        thread(RenderPixels).detach();
    }

}

int main() {
    ofstream file;
    file.open("example.ppm");
    file << "P3\n" << nx << " " << ny << "\n255\n";
    hitable *world = random_scene();
    vec3 look_from(13, 2, 3);
    vec3 look_at(0, 0, 0);
    float distance_to_focus = 10;
    float aperature = 0.1;
    camera camera(look_from, look_at, vec3(0, 1, 0), 20, float(nx) / float(ny), aperature, distance_to_focus);
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