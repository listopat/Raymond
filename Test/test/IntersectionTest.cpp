#include <catch.hpp>
#include <Intersection.h>
#include <Shapes/Sphere.h>
#include <Shapes/Plane.h>
#include <Hit.h>

#include <vector>

static const double EPSILON = 0.00001;

TEST_CASE("Intersections working as expected", "[intersection]") {
    SECTION("An intersection encapsulates t and object") {
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        Intersection i = Intersection::Intersection(3.5, s);
        REQUIRE(i.getT() == 3.5);
        REQUIRE(i.getObject() == s);
    }

    SECTION("Aggregating intersections") {
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        Intersection i1 = Intersection::Intersection(1.0, s);
        Intersection i2 = Intersection::Intersection(2.0, s);

        std::vector<Intersection> xs{i1, i2};
        REQUIRE(xs.size() == 2);
        REQUIRE(xs[0].getT() == 1.0);
        REQUIRE(xs[1].getT() == 2.0);
    }
    
    SECTION("The hit, when all intersections have positive t") {
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        Intersection i1 = Intersection::Intersection(1.0, s);
        Intersection i2 = Intersection::Intersection(2.0, s);

        std::vector<Intersection> xs{ i1, i2 };
        std::optional<Intersection> i = Intersection::hit(xs);
        REQUIRE(i.has_value());
        REQUIRE(i.value() == i1);
    }

    SECTION("The hit, when some intersections have negative t") {
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        Intersection i1 = Intersection::Intersection(-1.0, s);
        Intersection i2 = Intersection::Intersection(2.0, s);

        std::vector<Intersection> xs{ i1, i2 };
        std::optional<Intersection> i = Intersection::hit(xs);
        REQUIRE(i.has_value());
        REQUIRE(i.value() == i2);
    }

    SECTION("The hit, when all intersections have negative t") {
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        Intersection i1 = Intersection::Intersection(-1.0, s);
        Intersection i2 = Intersection::Intersection(-2.0, s);

        std::vector<Intersection> xs{ i1, i2 };
        std::optional<Intersection> i = Intersection::hit(xs);
        REQUIRE(!i.has_value());
    }
    
    SECTION("The hit is always the lowest nonnegative intersection") {
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        Intersection i1 = Intersection::Intersection(5, s);
        Intersection i2 = Intersection::Intersection(7, s);
        Intersection i3 = Intersection::Intersection(-3, s);
        Intersection i4 = Intersection::Intersection(2, s);
        std::vector<Intersection> xs{ i1, i2, i3, i4};
        std::optional<Intersection> i = Intersection::hit(xs);
        REQUIRE(i.has_value());
        REQUIRE(i.value() == i4);
    }

    SECTION("Precomputing the state of an intersection") {
        Ray r = Ray(Tuple::point(0, 0, -5), Tuple::vector(0, 0, 1));
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        Intersection i = Intersection::Intersection(4, s);
        Hit comps = i.prepareHit(r, { i });
        REQUIRE(comps.getT() == i.getT());
        REQUIRE(comps.getObject() == i.getObject());
        REQUIRE(comps.point == Tuple::point(0, 0, -1));
        REQUIRE(comps.eyev == Tuple::vector(0, 0, -1));
        REQUIRE(comps.normalv == Tuple::vector(0, 0, -1));
    }

    SECTION("The hit, when an intersection occurs on the outside") {
        Ray r = Ray(Tuple::point(0, 0, -5), Tuple::vector(0, 0, 1));
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        Intersection i = Intersection::Intersection(4, s);
        Hit comps = i.prepareHit(r, { i });
        REQUIRE(!comps.inside);
    }

    SECTION("The hit, when an intersection occurs on the inside") {
        Ray r = Ray(Tuple::point(0, 0, 0), Tuple::vector(0, 0, 1));
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        Intersection i = Intersection::Intersection(1, s);
        Hit comps = i.prepareHit(r, { i });
        REQUIRE(comps.inside);
        REQUIRE(comps.point == Tuple::point(0, 0, 1));
        REQUIRE(comps.eyev == Tuple::vector(0, 0, -1));
        REQUIRE(comps.normalv == Tuple::vector(0, 0, -1));
    }

    SECTION("The hit should offset the point") {
        Ray r = Ray(Tuple::point(0, 0, -5), Tuple::vector(0, 0, 1));
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        s->setTransform(Matrix::translation(0, 0, 1));
        Intersection i = Intersection(5, s);
        Hit h = i.prepareHit(r, { i });
        REQUIRE(h.overPoint[2] < EPSILON/2);
        REQUIRE(h.point[2] > h.overPoint[2]);
    }

    SECTION("Precomputing the reflection vector") {
        std::shared_ptr<Shape> plane = Plane::createPlane();
        Ray r = Ray::Ray(Tuple::point(0, 1, -1), Tuple::vector(0, -sqrt(2.0) / 2.0, sqrt(2.0) / 2.0));
        Intersection i = Intersection::Intersection(sqrt(2.0), plane);
        Hit h = i.prepareHit(r, { i });
        REQUIRE(h.reflectv == Tuple::vector(0, sqrt(2.0) / 2.0, sqrt(2.0) / 2.0));
    }

    SECTION("Finding n1 and n2 at various intersections") {
        std::shared_ptr<Sphere> A = Sphere::createGlassSphere();
        A->setTransform(Matrix::scaling(2, 2, 2));
        A->getMaterial().refractiveIndex = 1.5;
        std::shared_ptr<Sphere> B = Sphere::createGlassSphere();
        B->setTransform(Matrix::translation(0, 0, -0.25));
        B->getMaterial().refractiveIndex = 2.0;
        std::shared_ptr<Sphere> C = Sphere::createGlassSphere();
        C->setTransform(Matrix::translation(0, 0, 0.25));
        C->getMaterial().refractiveIndex = 2.5;

        Ray r = Ray(Tuple::point(0, 0, -4), Tuple::vector(0, 0, 1));
        std::vector<Intersection> intersections = {
            Intersection::Intersection(2, A),
            Intersection::Intersection(2.75, B),
            Intersection::Intersection(3.25, C),
            Intersection::Intersection(4.75, B),
            Intersection::Intersection(5.25, C),
            Intersection::Intersection(6, A),
        };

        Hit h0 = intersections[0].prepareHit(r, intersections);
        REQUIRE(h0.n1 == 1.0);
        REQUIRE(h0.n2 == 1.5);
        Hit h1 = intersections[1].prepareHit(r, intersections);
        REQUIRE(h1.n1 == 1.5);
        REQUIRE(h1.n2 == 2.0);
        Hit h2 = intersections[2].prepareHit(r, intersections);
        REQUIRE(h2.n1 == 2.0);
        REQUIRE(h2.n2 == 2.5);
        Hit h3 = intersections[3].prepareHit(r, intersections);
        REQUIRE(h3.n1 == 2.5);
        REQUIRE(h3.n2 == 2.5);
        Hit h4 = intersections[4].prepareHit(r, intersections);
        REQUIRE(h4.n1 == 2.5);
        REQUIRE(h4.n2 == 1.5);
        Hit h5 = intersections[5].prepareHit(r, intersections);
        REQUIRE(h5.n1 == 1.5);
        REQUIRE(h5.n2 == 1.0);
    }

    SECTION("The under point is offset below the surface") {
        Ray r = Ray(Tuple::point(0, 0, -5), Tuple::vector(0, 0, 1));
        std::shared_ptr<Sphere> glassSphere = Sphere::createGlassSphere();
        glassSphere->setTransform(Matrix::translation(0, 0, 1));
        Intersection i = Intersection::Intersection(5, glassSphere);
        Hit h = i.prepareHit(r, { i });
        REQUIRE(h.underPoint[2] > EPSILON / 2);
        REQUIRE(h.point[2] < h.underPoint[2]);
    }

    SECTION("The Schlick approximation under total internal reflection") {
        std::shared_ptr<Sphere> glassSphere = Sphere::createGlassSphere();
        Ray r = Ray(Tuple::point(0, 0, sqrt(2.0) / 2.0), Tuple::vector(0, 1, 0));
        std::vector<Intersection> intersections = {
            Intersection::Intersection(-sqrt(2.0) / 2.0, glassSphere),
            Intersection::Intersection(sqrt(2.0) / 2.0, glassSphere),
        };
        Hit h = intersections[1].prepareHit(r, intersections);
        double reflectance = h.schlick();
        REQUIRE(reflectance == 1.0);
    }

    SECTION("The Schlick approximation with a perpendicular viewing angle") {
        std::shared_ptr<Sphere> glassSphere = Sphere::createGlassSphere();
        Ray r = Ray(Tuple::point(0, 0, 0), Tuple::vector(0, 1, 0));
        std::vector<Intersection> intersections = {
            Intersection::Intersection(-1, glassSphere),
            Intersection::Intersection(1, glassSphere),
        };
        Hit h = intersections[1].prepareHit(r, intersections);
        double reflectance = h.schlick();
        REQUIRE(abs(reflectance - 0.04) < EPSILON);
    }

    SECTION("The Schlick approximation with small angle and n2 > n1") {
        std::shared_ptr<Sphere> glassSphere = Sphere::createGlassSphere();
        Ray r = Ray(Tuple::point(0, 0.99, -2), Tuple::vector(0, 0, 1));
        std::vector<Intersection> intersections = {
            Intersection::Intersection(1.8589, glassSphere),
        };
        Hit h = intersections[0].prepareHit(r, intersections);
        double reflectance = h.schlick();
        REQUIRE(abs(reflectance - 0.48873) < EPSILON);
    }
}