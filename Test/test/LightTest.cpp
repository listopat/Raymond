#include <catch.hpp>
#include <Lights/PointLight.h>
#include <Lights/AreaLight.h>

TEST_CASE("Point lights working as expected", "[pointlight]") {
    SECTION("A point light has a position and intensity") {
        Color intensity = Color::Color(1, 1, 1);
        Tuple position = Tuple::point(0, 0, 0);
        PointLight light = PointLight(position, intensity);
        REQUIRE((light.position == position));
        REQUIRE((light.intensity == intensity));
    }
}

TEST_CASE("Area lights working as expected", "[arealight]") {
    SECTION("Creating an area light") {
        Color intensity = Color::Color(1, 1, 1);
        Tuple corner = Tuple::point(0, 0, 0);
        Tuple v1 = Tuple::vector(2, 0, 0);
        Tuple v2 = Tuple::vector(0, 0, 1);
        AreaLight light = AreaLight(intensity, corner, v1, 4, v2, 2, false);
        REQUIRE(light.lowerLeftCorner == corner);
        REQUIRE(light.uVec == Tuple::vector(0.5, 0, 0));
        REQUIRE(light.uSteps == 4);
        REQUIRE(light.vVec == Tuple::vector(0, 0, 0.5));
        REQUIRE(light.vSteps == 2);
        REQUIRE(light.samples == 8);
        REQUIRE(light.center == Tuple::point(1, 0, 0.5));
    }

    SECTION("Finding a single point on an area light") {
        Tuple corner = Tuple::point(0, 0, 0);
        Color intensity = Color::Color(1, 1, 1);
        Tuple v1 = Tuple::vector(2, 0, 0);
        Tuple v2 = Tuple::vector(0, 0, 1);
        AreaLight light = AreaLight(intensity, corner, v1, 4, v2, 2, false);
        REQUIRE(light.sample(0, 0) == Tuple::point(0.25, 0, 0.25));
        REQUIRE(light.sample(1, 0) == Tuple::point(0.75, 0, 0.25));
        REQUIRE(light.sample(0, 1) == Tuple::point(0.25, 0, 0.75));
        REQUIRE(light.sample(2, 0) == Tuple::point(1.25, 0, 0.25));
        REQUIRE(light.sample(3, 1) == Tuple::point(1.75, 0, 0.75));
    }
}