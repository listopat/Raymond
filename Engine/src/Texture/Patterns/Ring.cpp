#include <Texture/Patterns/Ring.h>
#include <math.h>

Ring::Ring(const Color &s1, const Color &s2) : a(s1), b(s2) {}

std::shared_ptr<Ring> Ring::createRing(const Color &s1, const Color &s2)
{
    return std::make_shared<Ring>(Ring(s1, s2));
}

Color Ring::atUV(const UV &uv) const
{
    return ((int) sqrt(uv.u * uv.u + uv.v * uv.v)) % 2 == 0 ? a : b;
}