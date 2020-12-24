#include <Texture/UVMapping.h>

static const double EPSILON = 0.00001;
static const double PI = 3.14159265;

bool UV::operator==(const UV & other) const
{
    return abs(u - other.u) < EPSILON
        && abs(v - other.v) < EPSILON;
}

std::shared_ptr<SphericalMap> SphericalMap::createSphericalMap()
{
    return std::make_shared<SphericalMap>(SphericalMap());
}

UV SphericalMap::operator()(const Tuple & point)
{
    double theta = atan2(point[0], point[2]);
    Tuple vector = Tuple::vector(point[0], point[1], point[2]);
    double radius = Tuple::magnitude(vector);
    double phi = acos(point[1] / radius);
    double rawU = theta / (2 * PI);
    double u = 1 - (rawU + 0.5);
    double v = 1 - phi / PI;
    return UV{ u, v };
}

std::shared_ptr<PlanarMap> PlanarMap::createPlanarMap()
{
    return std::make_shared<PlanarMap>(PlanarMap());
}

UV PlanarMap::operator()(const Tuple & point)
{
    double u = point[0] - floor(point[0]);
    double v = point[2] - floor(point[2]);

    return UV{ u, v };
}
