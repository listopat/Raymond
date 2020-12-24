#include <Texture/Patterns/Checkers.h>
#include <math.h>

Checkers::Checkers(const int width, const int height, const Color &s1, const Color &s2) : width(width), height(height), a(s1), b(s2) {}

std::shared_ptr<Checkers> Checkers::createCheckers(const int width, const int height, const Color &s1, const Color &s2)
{
    return std::make_shared<Checkers>(Checkers(width, height, s1, s2));
}

Color Checkers::atUV(const UV &uv) const
{
    int u2 = (int) floor(uv.u * width);
    int v2 = (int) floor(uv.v * height);
    
    return (u2 + v2) % 2 == 0 ? a : b;
}