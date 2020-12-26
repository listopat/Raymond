#pragma once
#include <Canvas.h>
#include <string>

namespace ImageIOInterface {
    void saveToImage(const Canvas &canvas, std::string outputPath);
    Canvas canvasFromImage(std::string inputPath);
}

