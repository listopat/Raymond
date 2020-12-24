#include <Intersection.h>
#include <Shapes/Shape.h>
#include <Hit.h>

Intersection::Intersection(double value, const std::shared_ptr<const Shape>& o) : t(value), object(o)
{
}

double Intersection::getT() const
{
    return t;
}

const std::shared_ptr<const Shape>& Intersection::getObject() const
{
    return object;
}

bool Intersection::operator==(const Intersection &other) const
{
    return object == other.object && t == other.t;
}

std::optional<Intersection> Intersection::hit(const std::vector<Intersection> &intersections)
{
    if (intersections.empty()) {
        return std::nullopt;
    }

    const Intersection * result = nullptr;
    for (const Intersection &i : intersections) {
        if (i.t > 0 && (result == nullptr || i.t < result->t)) {
            result = &i;
        }
    }
    
    return (result == nullptr) ? std::nullopt : std::optional<Intersection>{*result};
}

Hit Intersection::prepareHit(const Ray & ray) const
{
    Tuple point = Ray::position(ray, t);
    Tuple eyev = -ray.direction;
    Tuple normalv = object->normalAt(point);
    bool inside;

    if (Tuple::dot(normalv, eyev) < 0) {
        inside = true;
        normalv = -normalv;
    }
    else {
        inside = false;
    }

    return Hit(*this, inside, point, eyev, normalv);
}
