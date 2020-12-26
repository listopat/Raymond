#pragma once
#include <Ray.h>
#include <vector>
#include <memory>
#include <optional>

class Shape;
class Hit;

class Intersection {
public:
    Intersection(double value, const std::shared_ptr<const Shape> &o);

    double getT() const;
    const std::shared_ptr<const Shape>& getObject() const;

    bool operator==(const Intersection &other) const;

    static std::optional<Intersection> hit(const std::vector<Intersection> &intersections);
    Hit prepareHit(const Ray &ray, const std::vector<Intersection>& intersections) const;

private:
    double t;
    std::shared_ptr<const Shape> object;
};