#include <Shapes/Cube.h>
#include <algorithm>

static const double EPSILON = 0.000001;

std::shared_ptr<Cube> Cube::createCube()
{
    return std::make_shared<Cube>(Cube());
}

bool Cube::operator==(const Shape & other) const
{
    return true;
}

void Cube::setTransform(const Matrix & m)
{
    transform = m;
    inverseTransform = Matrix::inverse(transform);
}

Matrix Cube::getTransform() const
{
    return transform;
}

Matrix Cube::getInverseTransform() const
{
    return inverseTransform;
}

void Cube::setMaterial(const Material & m)
{
    material = m;
}

Material & Cube::getMaterial()
{
    return material;
}

Material Cube::getMaterial() const
{
    return material;
}

Tuple Cube::normalAt(const Tuple & point) const
{
    Tuple objectPoint = inverseTransform * point;

    std::vector<double> components = { abs(objectPoint[0]), abs(objectPoint[1]), abs(objectPoint[2]) };
    int maxComponent = static_cast<int>(std::max_element(components.begin(), components.end()) - components.begin());

    Tuple result = Tuple::vector(0, 0, 0);
    result[maxComponent] = objectPoint[maxComponent];
    return result;
}

std::vector<Intersection> Cube::intersects(const Ray & r) const
{
     Ray ray = Ray::transform(r, inverseTransform);
     std::vector<Intersection> result;
     
     std::pair<double, double> x = checkAxis(ray.origin[0], ray.direction[0]);
     std::pair<double, double> y = checkAxis(ray.origin[1], ray.direction[1]);
     std::pair<double, double> z = checkAxis(ray.origin[2], ray.direction[2]);

     double tMin = std::max({x.first, y.first, z.first});
     double tMax = std::min({x.second, y.second, z.second});

     if (tMin > tMax) {
         return {};
     }

     Intersection i1 = Intersection::Intersection(tMin, shared_from_this());
     Intersection i2 = Intersection::Intersection(tMax, shared_from_this());
     return { i1, i2 };
}

Cube::Cube() : transform(Matrix::getIdentity4x4()), inverseTransform(Matrix::getIdentity4x4()), material(Material::Material()) {

}

std::pair<double, double> Cube::checkAxis(double origin, double direction) const
{
    double tMinNumerator = (-1 - origin);
    double tMaxNumerator = (1 - origin);

    double tMin, tMax;
    if (abs(direction) >= EPSILON) 
    {
        tMin = tMinNumerator / direction;
        tMax = tMaxNumerator / direction;
    } 
    else
    {
        tMin = tMinNumerator * INFINITY;
        tMax = tMaxNumerator * INFINITY;
    }

    if (tMin > tMax)
    {
        std::swap(tMin, tMax);
    }

    return std::make_pair(tMin, tMax);
}
