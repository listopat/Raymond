#include <Texture/UVMapping.h>
#include <stdexcept>

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

std::shared_ptr<CubicMap> CubicMap::createCubicMap()
{
    return std::make_shared<CubicMap>(CubicMap());
}

UV CubicMap::operator()(const Tuple& point)
{
    switch (faceFromPoint(point)) {
    case CubicMap::FACE::FRONT:
        return UVFront(point);
        break;
    case CubicMap::FACE::BACK:
        return UVBack(point);
        break;
    case CubicMap::FACE::LEFT:
        return UVLeft(point);
        break;
    case CubicMap::FACE::RIGHT:
        return UVRight(point);
        break;
    case CubicMap::FACE::UP:
        return UVUp(point);
        break;
    case CubicMap::FACE::DOWN:
        return UVDown(point);
        break;
    default:
        throw std::invalid_argument("Invalid cube face.");
        break;
    }
}

CubicMap::FACE CubicMap::faceFromPoint(const Tuple& point)
{
    double absX = abs(point[0]);
    double absY = abs(point[1]);
    double absZ = abs(point[2]);

    double maxComponent = std::max({ absX, absY, absZ });
    
    if (maxComponent == point[0]) return CubicMap::FACE::RIGHT;
    else if (maxComponent == -point[0]) return CubicMap::FACE::LEFT;
    else if (maxComponent == point[1]) return CubicMap::FACE::UP;
    else if (maxComponent == -point[1]) return CubicMap::FACE::DOWN;
    else if (maxComponent == point[2]) return CubicMap::FACE::FRONT;
    else return CubicMap::FACE::BACK;
}

UV CubicMap::UVFront(const Tuple& point)
{
    double u = fmod((point[0] + 1), 2.0) / 2.0;
    double v = fmod((point[1] + 1), 2.0) / 2.0;
    return UV{ u, v };
}

UV CubicMap::UVBack(const Tuple& point)
{
    double u = fmod((1 - point[0]), 2.0) / 2.0;
    double v = fmod((point[1] + 1), 2.0) / 2.0;
    return UV{ u, v };
}

UV CubicMap::UVLeft(const Tuple& point)
{
    double u = fmod((point[2] + 1), 2.0) / 2.0;
    double v = fmod((point[1] + 1), 2.0) / 2.0;
    return UV{ u, v };
}

UV CubicMap::UVRight(const Tuple& point)
{
    double u = fmod((1 - point[2]), 2.0) / 2.0;
    double v = fmod((point[1] + 1), 2.0) / 2.0;
    return UV{ u, v };
}

UV CubicMap::UVUp(const Tuple& point)
{
    double u = fmod((point[0] + 1), 2.0) / 2.0;
    double v = fmod((1 - point[2]), 2.0) / 2.0;
    return UV{ u, v };
}

UV CubicMap::UVDown(const Tuple& point)
{
    double u = fmod((point[0] + 1), 2.0) / 2.0;
    double v = fmod((point[2] + 1), 2.0) / 2.0;
    return UV{ u, v };
}
