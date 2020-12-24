#include <Shapes/Plane.h>

static const double EPSILON = 0.000001;

std::shared_ptr<Plane> Plane::createPlane()
{
    return std::make_shared<Plane>(Plane());
}

bool Plane::operator==(const Shape & other) const
{
    return true;
}

void Plane::setTransform(const Matrix & m)
{
    transform = m;
    inverseTransform = Matrix::inverse(transform);
}

Matrix Plane::getTransform() const
{
    return transform;
}

Matrix Plane::getInverseTransform() const
{
    return inverseTransform;
}

void Plane::setMaterial(const Material & m)
{
    material = m;
}

Material & Plane::getMaterial()
{
    return material;
}

Material Plane::getMaterial() const
{
    return material;
}

Tuple Plane::normalAt(const Tuple & point) const
{
    Tuple worldNormal = Matrix::transpose(inverseTransform) * Tuple::vector(0, 1, 0);
    worldNormal[3] = 0.0f;
    return Tuple::normalize(worldNormal);
}

std::vector<Intersection> Plane::intersects(const Ray & r) const
{
     Ray ray = Ray::transform(r, inverseTransform);
     std::vector<Intersection> result;
     
     if (abs(ray.direction[1]) >= EPSILON) {
         double t = -ray.origin[1] / ray.direction[1];
         result.push_back(Intersection(t, shared_from_this()));
     }

     return result;
}

Plane::Plane() : transform(Matrix::getIdentity4x4()), inverseTransform(Matrix::getIdentity4x4()), material(Material::Material()) {

}