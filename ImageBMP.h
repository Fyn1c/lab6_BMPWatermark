#pragma once
#include <iostream>
#include <vector>
#include <cstdint>
#include <string>

#pragma pack(push, 1)
struct Pixel {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
};
#pragma pack(pop)

class BMPImage {
private:
    int width;
    int height;
    int bitDepth;
    std::vector<Pixel> pixels;

public:
    BMPImage();

    bool load(const std::string& filename);
    bool save(const std::string& filename);

    Pixel& at(int x, int y) { return pixels[y * width + x]; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};