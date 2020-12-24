#pragma once
#include <Intersection.h>

class Hit : public Intersection {
public:
    const bool inside;
    const Tuple point;
    const Tuple eyev;
    const Tuple normalv;
    const Tuple overPoint;

    Hit(const Intersection &i, const bool ins, const Tuple &p, const Tuple &ev, const Tuple &nv);
};