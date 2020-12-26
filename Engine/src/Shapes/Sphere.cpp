#include <Shapes/Sphere.h>

std::shared_ptr<Sphere> Sphere::createSphere()
{
    return std::make_shared<Sphere>(Sphere());
}

std::shared_ptr<Sphere> Sphere::createGlassSphere()
{
    std::shared_ptr<Sphere> result = std::make_shared<Sphere>(Sphere());
    result->getMaterial().transparency = 1.0;
    result->getMaterial().refractiveIndex = 1.5;
    return result;
}

bool Sphere::operator==(const Shape &other) const
{
    return true;
}

void Sphere::setTransform(const Matrix &m)
{
    transform = m;
    inverseTransform = Matrix::inverse(transform);
}

Matrix Sphere::getTransform() const
{
    return transform;
}

Matrix Sphere::getInverseTransform() const
{
    return inverseTransform;
}

void Sphere::setMaterial(const Material &m)
{
    material = m;
}

Material & Sphere::getMaterial()
{
    return material;
}

Material Sphere::getMaterial() const
{
    return material;
}

Tuple Sphere::normalAt(const Tuple & point) const
{
    Tuple objectPoint = inverseTransform * point;
    Tuple objectNormal = Tuple::normalize(objectPoint - Tuple::point(0, 0, 0));
    Tuple worldNormal = Matrix::transpose(inverseTransform) * objectNormal;
    worldNormal[3] = 0.0f;
    return Tuple::normalize(worldNormal);
}

std::vector<Intersection> Sphere::intersects(const Ray & r) const
{
    Ray ray = Ray::transform(r, inverseTransform);
    Tuple sphereToRay = ray.origin - Tuple::point(0, 0, 0);
    double a = Tuple::dot(ray.direction, ray.direction);
    double b = 2 * Tuple::dot(ray.direction, sphereToRay);
    double c = Tuple::dot(sphereToRay, sphereToRay) - 1.0;

    double discriminant = (b * b) - (4 * a * c);

    if (discriminant < 0.0) {
        return std::vector<Intersection>{};
    }
    else {
        double t1 = (-b - sqrt(discriminant)) / (2 * a);
        double t2 = (-b + sqrt(discriminant)) / (2 * a);

        Intersection i1 = Intersection::Intersection(t1, shared_from_this());
        Intersection i2 = Intersection::Intersection(t2, shared_from_this());
        return i1.getT() > i2.getT() ? std::vector<Intersection>{i2, i1} : std::vector<Intersection>{ i1, i2 };
    }
}

Sphere::Sphere() : transform(Matrix::getIdentity4x4()), inverseTransform(Matrix::getIdentity4x4()), material(Material::Material())
{
}