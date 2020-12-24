#include <catch.hpp>
#include <Camera.h>
#include <World.h>
#include <Canvas.h>

static const double PI = 3.14159265;
static const double EPSILON = 0.00001;

TEST_CASE("Camera working properly", "[camera]") {
    SECTION("Constructing a camera") {
        int hsize = 160;
        int vsize = 120;
        double fieldOfView = PI / 2;
        Camera camera = Camera::makeCamera(hsize, vsize, fieldOfView);
        REQUIRE(camera.hSize == hsize);
        REQUIRE(camera.vSize == vsize);
        REQUIRE(camera.fieldOfView == fieldOfView);
        REQUIRE(camera.getTransform() == Matrix::identity4x4);
    }

    SECTION("The pixel size for a horizontal canvas") {
        Camera camera = Camera::makeCamera(200, 125, PI / 2);
        REQUIRE((camera.pixelSize - 0.01) < EPSILON);
    }

    SECTION("The pixel size for a horizontal canvas") {
        Camera camera = Camera::makeCamera(125, 200, PI / 2);
        REQUIRE((camera.pixelSize - 0.01) < EPSILON);
    }

    SECTION("Constructing a ray through the center of the canvas") {
        Camera camera = Camera::makeCamera(201, 101, PI / 2);
        Ray r = camera.rayForPixel(100, 50);
        REQUIRE(r.origin == Tuple::point(0, 0, 0));
        REQUIRE(r.direction == Tuple::vector(0, 0, -1));
    }

    SECTION("Constructing a ray through a corner of the canvas") {
        Camera camera = Camera::makeCamera(201, 101, PI / 2);
        Ray r = camera.rayForPixel(0, 0);
        REQUIRE(r.origin == Tuple::point(0, 0, 0));
        REQUIRE(r.direction == Tuple::vector(0.66519, 0.33259, -0.66851));
    }

    SECTION("Constructing a ray when the camera is transformed") {
        Camera camera = Camera::makeCamera(201, 101, PI / 2);
        camera.setTransform(Matrix::rotationY(PI / 4) * Matrix::translation(0, -2, 5));
        Ray r = camera.rayForPixel(100, 50);
        REQUIRE(r.origin == Tuple::point(0, 2, -5));
        REQUIRE(r.direction == Tuple::vector(sqrt(2)/2, 0, -sqrt(2)/2));
    }

    SECTION("Rendering a world with a camera") {
        World w = World::makeDefaultWorld();
        Camera camera = Camera::makeCamera(11, 11, PI / 2);
        Tuple from = Tuple::point(0, 0, -5);
        Tuple to = Tuple::point(0, 0, 0);
        Tuple up = Tuple::vector(0, 1, 0);
        camera.setTransform(Matrix::viewTransform(from, to, up));
        Canvas image = camera.render(w);
        REQUIRE(image.at(5, 5) == Color::Color(0.38066, 0.47583, 0.2855));
    }
}