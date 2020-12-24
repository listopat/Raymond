#include <catch.hpp>
#include <Shapes/Shape.h>
#include <Shapes/Sphere.h>

TEST_CASE("Rays working as expected", "[ray]") {
    SECTION("Creating and querying a ray") {
        Tuple origin = Tuple::point(1, 2, 3);
        Tuple direction = Tuple::vector(4, 5, 6);
        Ray r = Ray::Ray(origin, direction);
        REQUIRE((r.origin == origin));
    }

    SECTION("Computing a point from a distance") {
        Ray r = Ray::Ray(Tuple::point(2, 3, 4), Tuple::vector(1, 0, 0));
        REQUIRE((Ray::position(r, 0) == Tuple::point(2, 3, 4)));
        REQUIRE((Ray::position(r, 1) == Tuple::point(3, 3, 4)));
        REQUIRE((Ray::position(r, -1) == Tuple::point(1, 3, 4)));
        REQUIRE((Ray::position(r, 2.5) == Tuple::point(4.5f, 3, 4)));
    }

    SECTION("A ray intersects a sphere at two points") {
        Ray r = Ray::Ray(Tuple::point(0, 0, -5), Tuple::vector(0, 0, 1));
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        std::vector<Intersection> intersections = s->intersects(r);
        REQUIRE(intersections.size() == 2);
        REQUIRE(intersections[0].getT() == 4.0);
        REQUIRE(intersections[1].getT() == 6.0);
    }

    SECTION("A ray intersects a sphere at a tangent") {
        Ray r = Ray::Ray(Tuple::point(0, 1, -5), Tuple::vector(0, 0, 1));
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        std::vector<Intersection> intersections = s->intersects(r);
        REQUIRE(intersections.size() == 2);
        REQUIRE(intersections[0].getT() == 5.0);
        REQUIRE(intersections[1].getT() == 5.0);
    }

    SECTION("A ray misses a sphere") {
        Ray r = Ray::Ray(Tuple::point(0, 2, -5), Tuple::vector(0, 0, 1));
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        std::vector<Intersection> intersections = s->intersects(r);
        REQUIRE(intersections.size() == 0);
    }

    SECTION("A ray originates inside a sphere") {
        Ray r = Ray::Ray(Tuple::point(0, 0, 0), Tuple::vector(0, 0, 1));
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        std::vector<Intersection> intersections = s->intersects(r);
        REQUIRE(intersections.size() == 2);
        REQUIRE(intersections[0].getT() == -1.0);
        REQUIRE(intersections[1].getT() == 1.0);
    }

    SECTION("A sphere is behind a ray") {
        Ray r = Ray::Ray(Tuple::point(0, 0, 5), Tuple::vector(0, 0, 1));
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        std::vector<Intersection> intersections = s->intersects(r);
        REQUIRE(intersections.size() == 2);
        REQUIRE(intersections[0].getT() == -6.0);
        REQUIRE(intersections[1].getT() == -4.0);
    }

    SECTION("Intersect sets the object on the intersection") {
        Ray r = Ray::Ray(Tuple::point(0, 0, -5), Tuple::vector(0, 0, 1));
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        std::vector<Intersection> intersections = s->intersects(r);
        REQUIRE(intersections.size() == 2);
        REQUIRE(intersections[0].getObject() == s);
        REQUIRE(intersections[1].getObject() == s);
    }

    SECTION("Translating a ray") {
        Ray r = Ray::Ray(Tuple::point(1, 2, 3), Tuple::vector(0, 1, 0));
        Matrix m = Matrix::translation(3, 4, 5);
        Ray r2 = Ray::transform(r, m);
        REQUIRE(r2.origin == Tuple::point(4, 6, 8));
        REQUIRE(r2.direction == Tuple::vector(0, 1, 0));
    }

    SECTION("Scaling a ray") {
        Ray r = Ray::Ray(Tuple::point(1, 2, 3), Tuple::vector(0, 1, 0));
        Matrix m = Matrix::scaling(2, 3, 4);
        Ray r2 = Ray::transform(r, m);
        REQUIRE(r2.origin == Tuple::point(2, 6, 12));
        REQUIRE(r2.direction == Tuple::vector(0, 3, 0));
    }

    SECTION("Intersecting a scaled sphere with a ray") {
        Ray r = Ray::Ray(Tuple::point(0, 0, -5), Tuple::vector(0, 0, 1));
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        s->setTransform(Matrix::scaling(2, 2, 2));
        std::vector<Intersection> xs = s->intersects(r);
        REQUIRE(xs.size() == 2);
        REQUIRE(xs[0].getT() == 3);
        REQUIRE(xs[1].getT() == 7);
    }

    SECTION("Intersecting a translated sphere with a ray") {
        Ray r = Ray::Ray(Tuple::point(0, 0, -5), Tuple::vector(0, 0, 1));
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        s->setTransform(Matrix::translation(5, 0, 0));
        std::vector<Intersection> xs = s->intersects(r);
        REQUIRE(xs.size() == 0);
    }
}
