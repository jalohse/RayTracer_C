#include <iostream>
#include "lodepng.h"
using namespace std;

#ifndef RAYTRACER_RENDERIMAGE_H
#define RAYTRACER_RENDERIMAGE_H

class Color {
public:
    unsigned char r, g, b;
    static unsigned char ClampInt(int v) { return v<0 ? 0 : (v>255 ? 255 : v); }
    static unsigned char FloatToByte(float r) { return ClampInt(int(r*255+0.5f)); }
    void SetColor(float x, float y, float z) {
        r = FloatToByte(x);
        g = FloatToByte(y);
        b = FloatToByte(z);

    }
};

class RenderImage {
private:
    Color *image;
    int width, height;
    atomic<int> numRenderedPixels;
public:
    RenderImage() : image(NULL), width(0), height(0), numRenderedPixels(0) {}
    void Init(int w, int h) {
        width = w;
        height = h;
        if(image) delete [] image;
        image = new Color[width * height];
        ResetNumRenderedPixels();
    }
    int GetWidth() const { return width;};
    int GetHeight() const { return height;};
    Color* GetPixels() const { return image;};

    void ResetNumRenderedPixels() { numRenderedPixels = 0; }
    int GetNumRenderedPixels() { return  numRenderedPixels; }
    void IncrementNumRenderedPixels(int n) { numRenderedPixels += n; }
    bool isRenderDone() const { return numRenderedPixels >= width * height; }
    bool SaveImage(const char *filename) const { return SavePNG(filename, &image[0].r, 3);}

private:
    bool SavePNG(const char *filename, unsigned char *data, int compCount) const {
        LodePNGColorType colorType;
        switch (compCount) {
            case 1: colorType = LCT_GREY; break;
            case 3: colorType = LCT_RGB; break;
            default: return false;
        }
        unsigned int error = lodepng::encode(filename, data, width, height, colorType, 8);
        return error == 0;
    }
};

#endif //RAYTRACER_RENDERIMAGE_H
