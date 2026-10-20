#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>
#include <cmath>

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
    BMPImage() : width(0), height(0), bitDepth(24) {}

    bool load(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            std::cerr << "Не удалось открыть файл: " << filename << "\n";
            return false;
        }
        uint16_t bfType;
        uint32_t bfSize;
        uint16_t bfReserved1, bfReserved2;
        uint32_t bfOffBits;
        file.read(reinterpret_cast<char*>(&bfType), 2);
        if (bfType != 0x4D42) {
            std::cerr << "Это не BMP-файл!\n";
            return false;
        }
        file.read(reinterpret_cast<char*>(&bfSize), 4);
        file.read(reinterpret_cast<char*>(&bfReserved1), 2);
        file.read(reinterpret_cast<char*>(&bfReserved2), 2);
        file.read(reinterpret_cast<char*>(&bfOffBits), 4);
        uint32_t biSize;
        int32_t biWidth, biHeight;
        uint16_t biPlanes, biBitCount;
        uint32_t biCompression;
        file.read(reinterpret_cast<char*>(&biSize), 4);
        file.read(reinterpret_cast<char*>(&biWidth), 4);
        file.read(reinterpret_cast<char*>(&biHeight), 4);
        file.read(reinterpret_cast<char*>(&biPlanes), 2);
        file.read(reinterpret_cast<char*>(&biBitCount), 2);
        file.read(reinterpret_cast<char*>(&biCompression), 4);
        file.seekg(bfOffBits, std::ios::beg);
        if (biBitCount != 24 || biCompression != 0) {
            std::cerr << "Поддерживаются только 24-битные BMP без сжатия.\n";
            return false;
        }
        width = biWidth;
        bool isBottomUp = (biHeight > 0);
        height = std::abs(biHeight);
        int rowSize = width * 3;
        int padding = (4 - (rowSize % 4)) % 4;
        pixels.resize(width * height);
        for (int y = 0; y < height; ++y) {
            int targetY = isBottomUp ? (height - 1 - y) : y;
            for (int x = 0; x < width; ++x) {
                file.read(reinterpret_cast<char*>(&pixels[targetY * width + x]), sizeof(Pixel));
            }
            if (padding > 0) {
                file.seekg(padding, std::ios::cur);
            }
        }
        file.close();
        return true;
    }

    bool save(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            std::cerr << "Не удалось создать файл: " << filename << "\n";
            return false;
        }
        int rowSize = width * 3;
        int padding = (4 - (rowSize % 4)) % 4;
        int paddedRowSize = rowSize + padding;
        uint32_t pixelDataSize = paddedRowSize * height;
        uint32_t fileSize = 54 + pixelDataSize;
        uint32_t offset = 54;
        uint16_t bfType = 0x4D42;
        file.write(reinterpret_cast<const char*>(&bfType), 2);
        file.write(reinterpret_cast<const char*>(&fileSize), 4);
        uint16_t reserved = 0;
        file.write(reinterpret_cast<const char*>(&reserved), 2);
        file.write(reinterpret_cast<const char*>(&reserved), 2);
        file.write(reinterpret_cast<const char*>(&offset), 4);
        uint32_t biSize = 40;
        int32_t biWidth = width;
        int32_t biHeight = height;
        uint16_t biPlanes = 1;
        uint16_t biBitCount = 24;
        uint32_t biCompression = 0;
        uint32_t biSizeImage = pixelDataSize;
        int32_t biXPelsPerMeter = 2835;
        int32_t biYPelsPerMeter = 2835;
        uint32_t biClrUsed = 0;
        uint32_t biClrImportant = 0;
        file.write(reinterpret_cast<const char*>(&biSize), 4);
        file.write(reinterpret_cast<const char*>(&biWidth), 4);
        file.write(reinterpret_cast<const char*>(&biHeight), 4);
        file.write(reinterpret_cast<const char*>(&biPlanes), 2);
        file.write(reinterpret_cast<const char*>(&biBitCount), 2);
        file.write(reinterpret_cast<const char*>(&biCompression), 4);
        file.write(reinterpret_cast<const char*>(&biSizeImage), 4);
        file.write(reinterpret_cast<const char*>(&biXPelsPerMeter), 4);
        file.write(reinterpret_cast<const char*>(&biYPelsPerMeter), 4);
        file.write(reinterpret_cast<const char*>(&biClrUsed), 4);
        file.write(reinterpret_cast<const char*>(&biClrImportant), 4);
        std::vector<uint8_t> padBytes(padding, 0);
        for (int y = height - 1; y >= 0; --y) {
            for (int x = 0; x < width; ++x) {
                file.write(reinterpret_cast<const char*>(&pixels[y * width + x]), sizeof(Pixel));
            }
            if (padding > 0) {
                file.write(reinterpret_cast<const char*>(padBytes.data()), padding);
            }
        }
        file.close();
        return true;
    }

    Pixel& at(int x, int y) { return pixels[y * width + x]; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};
