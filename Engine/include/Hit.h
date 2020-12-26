#pragma once
#include <Intersection.h>

class Hit : public Intersection {
public:
    const bool inside;
    const Tuple point;
    const Tuple eyev;
    const Tuple normalv;
    const Tuple underPoint, overPoint;
    const Tuple reflectv;
    const double n1, n2;

    Hit(const Intersection &i, const bool ins, const Tuple &p, const Tuple &ev, const Tuple &nv, const Tuple &rv, const double n_1, const double n_2);

    double schlick();
};