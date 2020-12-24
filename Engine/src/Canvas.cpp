#include <Canvas.h>

Canvas::Canvas(int w, int h) : width(w) , height(h) {
    grid = std::unique_ptr<Color[]>(new Color[w * h]);
}

Color Canvas::at(int x, int y) const
{
    return grid[y * width + x];
}

void Canvas::set(int x, int y, Color color)
{
    grid[y * width + x] = color;
}
