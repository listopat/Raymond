#include <catch.hpp>
#include <World.h>
#include <Shapes/Shape.h>
#include <Shapes/Sphere.h>

TEST_CASE("World working as expected", "[world]") {
    SECTION("Creating a world") {
        World w = World();
        REQUIRE(w.getLightCount() == 0);
        REQUIRE(w.getObjectCount() == 0);
    }
    
    SECTION("The default world") {
        World w = World::makeDefaultWorld();

        std::shared_ptr<Sphere> s1 = Sphere::createSphere();
        s1->setMaterial(Material(Color(0.8, 1.0, 0.6), 0.1, 0.7, 0.2, 200));
        std::shared_ptr<Sphere> s2 = Sphere::createSphere();
        s2->setTransform(Matrix::scaling(0.5, 0.5, 0.5));

        REQUIRE(w.getObjectCount() == 2);
        REQUIRE((w.getObject(0)->getMaterial() == s1->getMaterial()));
        REQUIRE((w.getObject(1)->getTransform() == s2->getTransform()));
        REQUIRE(w.getLightCount() == 1);
        REQUIRE(w.getPointLight(0).intensity == Color(1, 1, 1));
        REQUIRE(w.getPointLight(0).position == Tuple::point(-10, 10, -10));
    }
    
    
    SECTION("Intersect a world with a ray") {
        World w = World::makeDefaultWorld();
        Ray r = Ray(Tuple::point(0, 0, -5), Tuple::vector(0, 0, 1));
        std::vector<Intersection> xs = w.intersects(r);
        REQUIRE(xs.size() == 4);
        REQUIRE(xs[0].getT() == 4);
        REQUIRE(xs[1].getT() == 4.5);
        REQUIRE(xs[2].getT() == 5.5);
        REQUIRE(xs[3].getT() == 6);
    }

    SECTION("Shading an intersection") {
        World w = World::makeDefaultWorld();
        Ray r = Ray(Tuple::point(0, 0, -5), Tuple::vector(0, 0, 1));
        std::shared_ptr<Shape> s = w.getObject(0);
        Intersection i = Intersection(4, s);
        Hit comps = i.prepareHit(r);
        Color c = w.shadeHit(comps);
        REQUIRE(c == Color(0.38066, 0.47583, 0.2855));
    }

    SECTION("Shading an intersection from the inside") {
        World w = World::makeDefaultWorld();
        w.getPointLight(0).position = Tuple::point(0, 0.25, 0);
        Ray r = Ray(Tuple::point(0, 0, 0), Tuple::vector(0, 0, 1));
        std::shared_ptr<Shape> s = w.getObject(1);
        Intersection i = Intersection(0.5, s);
        Hit comps = i.prepareHit(r);
        Color c = w.shadeHit(comps);
        REQUIRE(c == Color(0.90498, 0.90498, 0.90498));
    }

    SECTION("The color when a ray misses") {
        World w = World::makeDefaultWorld();
        Ray r = Ray(Tuple::point(0, 0, -5), Tuple::vector(0, 1, 0));
        Color c = w.colorAt(r);
        REQUIRE(c == Color(0, 0, 0));
    }

    SECTION("The color when a ray hits") {
        World w = World::makeDefaultWorld();
        Ray r = Ray(Tuple::point(0, 0, -5), Tuple::vector(0, 0, 1));
        Color c = w.colorAt(r);
        REQUIRE(c == Color(0.38066, 0.47583, 0.2855));
    }

    SECTION("The color with an intersection behind the ray") {
        World w = World::makeDefaultWorld();
        w.getObject(0)->getMaterial().ambient = 1;
        w.getObject(1)->getMaterial().ambient = 1;
        Ray r = Ray(Tuple::point(0, 0, 0.75), Tuple::vector(0, 0, -1));
        Color c = w.colorAt(r);
        REQUIRE(c == w.getObject(1)->getMaterial().color);
    }

    SECTION("shadeHit() is given an intersection in shadow") {
        World w = World();
        w.addLight(PointLight(Tuple::point(0, 0, -10), Color(1, 1, 1)));
        w.addObject(Sphere::createSphere());
        std::shared_ptr<Sphere> s2 = Sphere::createSphere();
        s2->setTransform(Matrix::translation(0, 0, 10));
        w.addObject(s2);
        Ray r = Ray(Tuple::point(0, 0, 5), Tuple::vector(0, 0, 1));
        Intersection i = Intersection(4, s2);
        Hit h = i.prepareHit(r);
        Color c = w.shadeHit(h);
        REQUIRE(c == Color(0.1, 0.1, 0.1));
    }

    SECTION("isShadowed() tests for occlusion between two points") {
        World w = World::makeDefaultWorld();
        Tuple lightPosition = Tuple::point(-10, -10, -10);
        REQUIRE(!w.isShadowed(lightPosition, Tuple::point(-10, -10, 10)));
        REQUIRE(w.isShadowed(lightPosition, Tuple::point(10, 10, 10)));
        REQUIRE(!w.isShadowed(lightPosition, Tuple::point(-20, -20, -20)));
        REQUIRE(!w.isShadowed(lightPosition, Tuple::point(-5, -5, -5)));
    }

    SECTION("Point lights evaluate the light intensity at a given point") {
        World w = World::makeDefaultWorld();
        REQUIRE(w.intensityAt(w.getPointLight(0), Tuple::point(0, 1.00001, 0)) == 1.0);
        REQUIRE(w.intensityAt(w.getPointLight(0), Tuple::point(-1.0001, 0, 0)) == 1.0);
        REQUIRE(w.intensityAt(w.getPointLight(0), Tuple::point(0, 0, -1.0001)) == 1.0);
        REQUIRE(w.intensityAt(w.getPointLight(0), Tuple::point(0, 0, 1.0001)) == 0.0);
        REQUIRE(w.intensityAt(w.getPointLight(0), Tuple::point(1.0001, 0, 0)) == 0.0);
        REQUIRE(w.intensityAt(w.getPointLight(0), Tuple::point(0, -1.0001, 0)) == 0.0);
        REQUIRE(w.intensityAt(w.getPointLight(0), Tuple::point(0, 0, 0)) == 0.0);
    }

    SECTION("The area light intensity function") {
        World w = World::makeDefaultWorld();

        Tuple corner = Tuple::point(-0.5, -0.5, -5);
        Color intensity = Color::Color(1, 1, 1);
        Tuple v1 = Tuple::vector(1, 0, 0);
        Tuple v2 = Tuple::vector(0, 1, 0);
        AreaLight light = AreaLight(intensity, corner, v1, 2, v2, 2, false);

        REQUIRE(w.intensityAt(light, Tuple::point(0, 0, 2)) == 0.0);
        REQUIRE(w.intensityAt(light, Tuple::point(1, -1, 2)) == 0.25);
        REQUIRE(w.intensityAt(light, Tuple::point(1.5, 0, 2)) == 0.5);
        REQUIRE(w.intensityAt(light, Tuple::point(1.25, 1.25, 3)) == 0.75);
        REQUIRE(w.intensityAt(light, Tuple::point(0, 0, -2)) == 1.0);
    }
}