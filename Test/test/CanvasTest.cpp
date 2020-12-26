#include <catch.hpp>
#include <Canvas.h>
#include <Color.h>
#include <ImageIOInterface.h>

#include <sstream>

TEST_CASE("Canvas working properly", "[canvas]") {
    SECTION("Creating a canvas") {
        Canvas c = Canvas::Canvas(10, 20);
        REQUIRE(c.width == 10);
        REQUIRE(c.height == 20);

        bool isEveryPixelBlack = true;
        for (int i = 0; i < c.width; i++) {
            for (int j = 0; j < c.height; j++) {
                if (!(c.at(i, j) == Color::Color(0, 0, 0))) {
                    isEveryPixelBlack = false;
                }
            }
        }
        REQUIRE(isEveryPixelBlack);
    }

    SECTION("Writing pixels to a canvas") {
        Canvas c = Canvas::Canvas(10, 20);
        Color red = Color::Color(1, 0, 0);
        c.set(2, 3, red);
        REQUIRE((c.at(2, 3) == Color::Color(1, 0, 0)));
    }
}