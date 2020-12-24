#include <catch.hpp>
#include <Intersection.h>
#include <Shapes/Sphere.h>
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
        Hit comps = i.prepareHit(r);
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
        Hit comps = i.prepareHit(r);
        REQUIRE(!comps.inside);
    }

    SECTION("The hit, when an intersection occurs on the inside") {
        Ray r = Ray(Tuple::point(0, 0, 0), Tuple::vector(0, 0, 1));
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        Intersection i = Intersection::Intersection(1, s);
        Hit comps = i.prepareHit(r);
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
        Hit h = i.prepareHit(r);
        REQUIRE(h.overPoint[2] < EPSILON/2);
        REQUIRE(h.point[2] > h.overPoint[2]);
    }
}