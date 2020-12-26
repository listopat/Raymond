#include <Hit.h>

static const double EPSILON = 0.00001;

Hit::Hit(const Intersection & i, const bool ins, const Tuple & p, const Tuple & ev, const Tuple & nv, const Tuple &rv, const double n_1, const double n_2) :
    Intersection(i), inside(ins) , point(p), eyev(ev), normalv(nv), underPoint(p - nv * EPSILON), overPoint(p + nv * EPSILON), reflectv(rv), n1(n_1), n2(n_2)
{
}

double Hit::schlick()
{
    double cos = Tuple::dot(eyev, normalv);
    if (n1 > n2)
    {
        double n = n1 / n2;
        double sin2_t = pow(n, 2) * (1 - pow(cos, 2));
        if (sin2_t > 1.0)
        {
            return 1.0;
        }

        double cos_t = sqrt(1.0 - sin2_t);
        cos = cos_t;
    }

    double r0 = pow((n1 - n2) / (n1 + n2), 2);
    return r0 + (1 - r0) * pow((1 - cos), 5);
}
