#include <Hit.h>

static const double EPSILON = 0.00001;

Hit::Hit(const Intersection & i, const bool ins, const Tuple & p, const Tuple & ev, const Tuple & nv) :
    Intersection(i), inside(ins) , point(p), eyev(ev), normalv(nv), overPoint(p + nv * EPSILON)
{
}
