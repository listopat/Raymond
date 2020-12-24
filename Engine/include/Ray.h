#pragma once
#include <Tuple.h>
#include <Matrix.h>

class Ray {
public:
    const Tuple origin;
    const Tuple direction;

    Ray(const Tuple &o, const Tuple &d);

    static Tuple position(const Ray &r, double t);
    static Ray transform(const Ray &r, const Matrix &m);
};