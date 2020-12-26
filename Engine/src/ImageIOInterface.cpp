#include <ImageIOInterface.h>
#include <algorithm>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdexcept>

void ImageIOInterface::saveToImage(const Canvas& canvas, std::string outputPath)
{
    uint8_t* pixels = new uint8_t[canvas.width * canvas.height * 3];

    int index = 0;
    for (int j = 0; j < canvas.height; j++)
    {
        for (int i = 0; i < canvas.width; i++)
        {
            Color color = canvas.at(i, j);
            uint8_t r = (int)round(255.0 * std::clamp(color.red, 0.0, 1.0));
            uint8_t g = (int)round(255.0 * std::clamp(color.green, 0.0, 1.0));
            uint8_t b = (int)round(255.0 * std::clamp(color.blue, 0.0, 1.0));

            pixels[index++] = r;
            pixels[index++] = g;
            pixels[index++] = b;
        }
    }

    stbi_write_jpg(outputPath.c_str(), canvas.width, canvas.height, 3, pixels, 100);
    delete[] pixels;
}


Canvas ImageIOInterface::canvasFromImage(std::string inputPath)
{
    int width, height, channels;
    unsigned char* img = stbi_load(inputPath.c_str(), &width, &height, &channels, STBI_rgb);

    if (img == NULL) {
        throw std::invalid_argument("Error on loading image: " + inputPath);
    }

    Canvas result = Canvas(width, height);

    int currentPixel = 0;
    for (int y = 0; y < result.height; y++) {
        for (int x = 0; x < result.width; x++) {
            Color color = Color(img[currentPixel]/255.0, img[currentPixel+1]/255.0, img[currentPixel+2]/255.0);
            result.set(x, y, color);
            currentPixel += 3;
        }
    }

    stbi_image_free(img);

    return result;
}