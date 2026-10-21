#include "Watermark.h"
#include <stdexcept>
#include <cstdint>

//функция смешения 1 цвета 
static std::uint8_t ColorMix(std::uint8_t orig, std::uint8_t mark, double alpha) {
    double value = orig * (1.0 - alpha) + mark * alpha;
    return (std::uint8_t)(value + 0.5);
}

//функция смешения 2 пикселей
Pixel BlendPixel(const Pixel& orig, const Pixel& mark, double alpha) {
    Pixel result;
    result.red = ColorMix(orig.red, mark.red, alpha);
    result.green = ColorMix(orig.green, mark.green, alpha);
    result.blue = ColorMix(orig.blue, mark.blue, alpha);
    return result;
}

// наложение вотермакрки на файл
BMPImage WatermarkMake(BMPImage base, BMPImage& mark, double alpha) {
    if (alpha < 0.0 || alpha > 1.0) {
        throw std::invalid_argument("alpha must be in range [0;1]");
    }
    if (mark.getWidth() > base.getWidth() || mark.getHeight() > base.getHeight()) {
        throw std::invalid_argument("Waternark is larger than image");
    }

    const int coordinateX = base.getWidth() - mark.getWidth();
    const int coordinateY = base.getHeight() - mark.getHeight();

    for (int y = 0; y < mark.getHeight(); y++) {
        for (int x = 0; x < mark.getWidth(); x++) {
            Pixel& base_same = base.at(x + coordinateX, y + coordinateY);
            base_same = BlendPixel(base_same, mark.at(x, y), alpha);
        }
    }
    return base;
}