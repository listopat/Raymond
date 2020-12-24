#include <Texture/Patterns/Gradient.h>

Gradient::Gradient(const Color &s1, const Color &s2) : a(s1), b(s2) {}

std::shared_ptr<Gradient> Gradient::createGradient(const Color & s1, const Color & s2)
{
    return std::make_shared<Gradient>(Gradient(s1, s2));
}

Color Gradient::atUV(const UV &uv) const
{
    Color colorDifference = b - a;
    double fraction = uv.u - floor(uv.u);
    return a + colorDifference * fraction;
}
