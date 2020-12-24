#include <Ray.h>
#include <math.h>
#include <stdexcept>

Ray::Ray(const Tuple &o, const Tuple &d) : origin(o), direction(d)
{
    if (!Tuple::isPoint(origin)) {
        throw std::invalid_argument("Origin must be a point.");
    }

    if (!Tuple::isVector(direction)) {
        throw std::invalid_argument("Direction must be a vector");
    }
}

Tuple Ray::position(const Ray &r, double t)
{
    return r.origin + (r.direction * t);
}

Ray Ray::transform(const Ray & r, const Matrix & m)
{
    return Ray(m * r.origin, m * r.direction);
}
