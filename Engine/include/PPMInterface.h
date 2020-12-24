#pragma once
#include <Canvas.h>
#include <string>

namespace PPMInterface {
    std::string canvasToPPM(const Canvas &canvas);
    void PPMToDisk(std::string ppm, std::string path);

    Canvas canvasFromPPM(std::string &ppm);
}

