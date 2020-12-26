#include <catch.hpp>
#include <Shapes/Sphere.h>
#include <Shapes/Plane.h>
#include <Shapes/Cube.h>

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

    SECTION("A helper for producing a sphere with a glassy material") {
        std::shared_ptr<Sphere> s = Sphere::createGlassSphere();
        REQUIRE(s->getTransform() == Matrix::identity4x4);
        REQUIRE(s->getMaterial().transparency == 1.0);
        REQUIRE(s->getMaterial().refractiveIndex == 1.5);
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

TEST_CASE("Cube tests", "[cube]") {
    SECTION("A ray intersects a cube") {
        std::shared_ptr<Cube> c = Cube::createCube();

        Ray r1 = Ray(Tuple::point(5, 0.5, 0), Tuple::vector(-1, 0, 0));
        Ray r2 = Ray(Tuple::point(-5, 0.5, 0), Tuple::vector(1, 0, 0));
        Ray r3 = Ray(Tuple::point(0.5, 5, 0), Tuple::vector(0, -1, 0));
        Ray r4 = Ray(Tuple::point(0.5, -5, 0), Tuple::vector(0, 1, 0));
        Ray r5 = Ray(Tuple::point(0.5, 0, 5), Tuple::vector(0, 0, -1));
        Ray r6 = Ray(Tuple::point(0.5, 0, -5), Tuple::vector(0, 0, 1));
        Ray r7 = Ray(Tuple::point(0, 0.5, 0), Tuple::vector(0, 0, 1));

        std::vector<Intersection> xs = c->intersects(r1);
        REQUIRE(xs.size() == 2);
        REQUIRE(xs[0].getT() == 4);
        REQUIRE(xs[1].getT() == 6);
        xs = c->intersects(r2);
        REQUIRE(xs.size() == 2);
        REQUIRE(xs[0].getT() == 4);
        REQUIRE(xs[1].getT() == 6);
        xs = c->intersects(r3);
        REQUIRE(xs.size() == 2);
        REQUIRE(xs[0].getT() == 4);
        REQUIRE(xs[1].getT() == 6);
        xs = c->intersects(r4);
        REQUIRE(xs.size() == 2);
        REQUIRE(xs[0].getT() == 4);
        REQUIRE(xs[1].getT() == 6);
        xs = c->intersects(r5);
        REQUIRE(xs.size() == 2);
        REQUIRE(xs[0].getT() == 4);
        REQUIRE(xs[1].getT() == 6);
        xs = c->intersects(r6);
        REQUIRE(xs.size() == 2);
        REQUIRE(xs[0].getT() == 4);
        REQUIRE(xs[1].getT() == 6);
        xs = c->intersects(r7);
        REQUIRE(xs.size() == 2);
        REQUIRE(xs[0].getT() == -1);
        REQUIRE(xs[1].getT() == 1);
    }

    SECTION("A ray misses a cube") {
        std::shared_ptr<Cube> c = Cube::createCube();

        Ray r1 = Ray(Tuple::point(-2, 0, 0), Tuple::vector(0.2673, 0.5345, 0.8018));
        Ray r2 = Ray(Tuple::point(0, -2, 0), Tuple::vector(0.8018, 0.2673, 0.5345));
        Ray r3 = Ray(Tuple::point(0, 0, -2), Tuple::vector(0.5345, 0.8018, 0.2673));
        Ray r4 = Ray(Tuple::point(2, 0, 2), Tuple::vector(0, 0, -1));
        Ray r5 = Ray(Tuple::point(0, 2, 2), Tuple::vector(0, -1, 0));
        Ray r6 = Ray(Tuple::point(2, 2, 0), Tuple::vector(-1, 0, 0));

        std::vector<Intersection> xs = c->intersects(r1);
        REQUIRE(xs.size() == 0);
        xs = c->intersects(r2);
        REQUIRE(xs.size() == 0);
        xs = c->intersects(r3);
        REQUIRE(xs.size() == 0);
        xs = c->intersects(r4);
        REQUIRE(xs.size() == 0);
        xs = c->intersects(r5);
        REQUIRE(xs.size() == 0);
        xs = c->intersects(r6);
        REQUIRE(xs.size() == 0);
    }

    SECTION("The normal on the surface of a cube") {
        std::shared_ptr<Cube> c = Cube::createCube();
        REQUIRE(c->normalAt(Tuple::point(1, 0.5, -0.8)) == Tuple::vector(1, 0, 0));
        REQUIRE(c->normalAt(Tuple::point(-1, -0.2, 0.9)) == Tuple::vector(-1, 0, 0));
        REQUIRE(c->normalAt(Tuple::point(-0.4, 1, -0.1)) == Tuple::vector(0, 1, 0));
        REQUIRE(c->normalAt(Tuple::point(0.3, -1, -0.7)) == Tuple::vector(0, -1, 0));
        REQUIRE(c->normalAt(Tuple::point(-0.6, 0.3, 1)) == Tuple::vector(0, 0, 1));
        REQUIRE(c->normalAt(Tuple::point(0.4, 0.4, -1)) == Tuple::vector(0, 0, -1));
        REQUIRE(c->normalAt(Tuple::point(1, 1, 1)) == Tuple::vector(1, 0, 0));
        REQUIRE(c->normalAt(Tuple::point(-1, -1, -1)) == Tuple::vector(-1, 0, 0));
    }
}