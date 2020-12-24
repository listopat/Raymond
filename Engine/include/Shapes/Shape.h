#pragma once
#include <Matrix.h>
#include <Material.h>
#include <Intersection.h>
#include <Ray.h>
#include <vector>

class Shape {
public:
    virtual bool operator==(const Shape &other) const = 0;
    virtual void setTransform(const Matrix &m) = 0;
    virtual Matrix getTransform() const = 0;
    virtual Matrix getInverseTransform() const = 0;
    virtual void setMaterial(const Material &m) = 0;
    virtual Material& getMaterial() = 0;
    virtual Material getMaterial() const = 0;
    virtual Tuple normalAt(const Tuple &point) const = 0;
    virtual std::vector<Intersection> intersects(const Ray &r) const = 0;
};