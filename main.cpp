#include <iostream>
#include <fstream>
using namespace std;

int main() {
    int nx = 200;
    int ny = 100;
    ofstream file;
    file.open("example.ppm");
    file << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny-1; j >= 0 ; j--) {
        for (int i = 0; i < nx; ++i) {
            float red = float(i) / float(nx);
            float green = float(j) / float(ny);
            float blue = 0.2;
            int ir = int(255.99 * red);
            int ig = int(255.99 * green);
            int ib = int(255.99 * blue);
            file << ir << " " << ig << " " << ib << "\n";
        }
    }
    file.close();
    return 0;
}