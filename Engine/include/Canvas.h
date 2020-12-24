#pragma once
#include <array>
#include <memory>
#include <Color.h>

class Canvas {
public:
    const int width, height;

    Canvas(int w, int h);
    
    Color at(int x, int y) const;
    void set(int x, int y, Color color);

private:
    std::unique_ptr<Color[]> grid;
};