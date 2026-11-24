#include <iostream>
#include "ImageBMP.h"
#include "Watermark.h"

int main(int argc, char** argv){

    if(argc != 4){
        std::cout << "Input should be: input_path mark_path output_path" << std::endl;
        return 1;
    }

    BMPImage image{};
    BMPImage mark{};

    std::string input = argv[1];
    std::string mark_path = argv[2];
    std::string output = argv[3];

    image.load(input);
    mark.load(mark_path);
    image = WatermarkMake(image, mark);

    image.save(output);

    return 0;
}