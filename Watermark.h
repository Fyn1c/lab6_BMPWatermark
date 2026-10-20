#pragma once
#include <ImageBMP.h>

// смешивает 2 пискселя
Pixel BlendPixel(const Pixel& orig, const Pixel& mark, double alpha);

// наложение вотермарки
BMPImage WatermarkMake(BMPImage& orig, BMPImage& mark, double alpha = 0.4);