#include <catch.hpp>
#include <Shapes/Sphere.h>
#include <Shapes/Plane.h>

static const double PI = 3.14159265;

TEST_CASE("Shapes working as expected", "[shape]") {
    SECTION("The default transformation") {
        std::shared_ptr<Shape> s = Sphere::createSphere();
        REQUIRE((s->getTransform() == Matrix::identity4x4));
    }

    SECTION("Assigning a transformation") {
        std::shared_ptr<Shape> s = Sphere::createSphere();
        Matrix t = Matrix::translation(2, 3, 4);
        s->setTransform(t);
        REQUIRE((s->getTransform() == t));
    }

    SECTION("A shape has a default material") {
        std::shared_ptr<Shape> s = Sphere::createSphere();
        REQUIRE((s->getMaterial() == Material::Material()));
    }

    SECTION("A shape may be assigned a material") {
        std::shared_ptr<Shape> s = Sphere::createSphere();
        Material m = Material();
        m.ambient = 1;
        s->setMaterial(m);
        REQUIRE((s->getMaterial() == m));
    }

    SECTION("Computing the normal on a translated Shape") {
        std::shared_ptr<Shape> s = Sphere::createSphere();
        s->setTransform(Matrix::translation(0, 1, 0));
        Tuple n = s->normalAt(Tuple::point(0.0f, 1.70711, -0.70711));
        REQUIRE((n == Tuple::vector(0.0f, 0.70711, -0.70711)));
    }
    SECTION("Computing the normal on a transformed Shape") {
        std::shared_ptr<Shape> s = Sphere::createSphere();
        s->setTransform(Matrix::scaling(1, 0.5, 1) * Matrix::rotationZ(PI / 5));
        Tuple n = s->normalAt(Tuple::point(0.0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0));
        REQUIRE((n == Tuple::vector(0.0, 0.97014f, -0.24254)));
    }
}

TEST_CASE("Spheres working as expected", "[sphere]") {
    SECTION("The normal on a sphere at  a point on the x axis") {
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        Tuple n = s->normalAt(Tuple::point(1, 0, 0));
        REQUIRE((n == Tuple::vector(1, 0, 0)));
    }

    SECTION("The normal on a sphere at  a point on the y axis") {
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        Tuple n = s->normalAt(Tuple::point(0, 1, 0));
        REQUIRE((n == Tuple::vector(0, 1, 0)));
    }

    SECTION("The normal on a sphere at  a point on the z axis") {
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        Tuple n = s->normalAt(Tuple::point(0, 0, 1));
        REQUIRE((n == Tuple::vector(0, 0, 1)));
    }

    SECTION("The normal on a sphere at a nonaxial point") {
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        Tuple n = s->normalAt(Tuple::point(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0));
        REQUIRE((n == Tuple::vector(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0)));
    }

    SECTION("The normal is a normalized vector") {
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        Tuple n = s->normalAt(Tuple::point(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0));
        REQUIRE((n == Tuple::normalize(n)));
    }

    SECTION("Computing the normal on a translated sphere") {
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        s->setTransform(Matrix::translation(0, 1, 0));
        Tuple n = s->normalAt(Tuple::point(0.0f, 1.70711, -0.70711));
        REQUIRE((n == Tuple::vector(0.0f, 0.70711, -0.70711)));
    }
    SECTION("Computing the normal on a transformed sphere") {
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        s->setTransform(Matrix::scaling(1, 0.5, 1) * Matrix::rotationZ(PI / 5));
        Tuple n = s->normalAt(Tuple::point(0.0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0));
        REQUIRE((n == Tuple::vector(0.0, 0.97014f, -0.24254)));
    }
}

TEST_CASE("Planes working as expected", "[plane]") {
    SECTION("The normal of a plane is constant everywhere") {
        std::shared_ptr<Plane> s = Plane::createPlane();
        REQUIRE(s->normalAt(Tuple::point(0, 0, 0)) == Tuple::vector(0, 1, 0));
        REQUIRE(s->normalAt(Tuple::point(10, 0, -10)) == Tuple::vector(0, 1, 0));
        REQUIRE(s->normalAt(Tuple::point(-5, 0, 150)) == Tuple::vector(0, 1, 0));
    }

    SECTION("Intersect with a ray parallel to the plane") {
        std::shared_ptr<Plane> p = Plane::createPlane();
        Ray r = Ray(Tuple::point(0, 10, 0), Tuple::vector(0, 0, 1));
        std::vector<Intersection> xs = p->intersects(r);
        REQUIRE(xs.size() == 0);
    }

    SECTION("Intersect with a coplanar ray") {
        std::shared_ptr<Plane> p = Plane::createPlane();
        Ray r = Ray(Tuple::point(0, 0, 0), Tuple::vector(0, 0, 1));
        std::vector<Intersection> xs = p->intersects(r);
        REQUIRE(xs.size() == 0);
    }

    SECTION("A ray intersecting a plane from above") {
        std::shared_ptr<Plane> p = Plane::createPlane();
        Ray r = Ray(Tuple::point(0, 1, 0), Tuple::vector(0, -1, 0));
        std::vector<Intersection> xs = p->intersects(r);
        REQUIRE(xs.size() == 1);
        REQUIRE(xs[0].getT() == 1);
        REQUIRE(xs[0].getObject() == p);
    }

    SECTION("A ray intersecting a plane from below") {
        std::shared_ptr<Plane> p = Plane::createPlane();
        Ray r = Ray(Tuple::point(0, -1, 0), Tuple::vector(0, 1, 0));
        std::vector<Intersection> xs = p->intersects(r);
        REQUIRE(xs.size() == 1);
        REQUIRE(xs[0].getT() == 1);
        REQUIRE(xs[0].getObject() == p);
    }
}