#pragma once
#include <cstdio>

class ImageBMP{

    public:
        void load(std::FILE* file);
        void save(std::FILE* file);

};