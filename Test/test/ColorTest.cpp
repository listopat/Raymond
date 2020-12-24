#include <catch.hpp>
#include <Color.h>

TEST_CASE("Color operations working correctly", "[color]") {
    SECTION("Colors are (red, green, blue) tuples") {
        Color c = Color::Color(-0.5, 0.4, 1.7);
        REQUIRE(c.red == -0.5);
        REQUIRE(c.green == 0.4);
        REQUIRE(c.blue == 1.7);
    }

    SECTION("Adding colors") {
        Color c1 = Color::Color(0.9, 0.6, 0.75);
        Color c2 = Color::Color(0.7, 0.1, 0.25);
        REQUIRE(((c1 + c2) == Color::Color(1.6, 0.7, 1)));
    }

    SECTION("Subtracting colors") {
        Color c1 = Color::Color(0.9, 0.6, 0.75);
        Color c2 = Color::Color(0.7, 0.1, 0.25);
        REQUIRE(((c1 - c2) == Color::Color(0.2, 0.5, 0.5)));
    }

    SECTION("Multiplying a color by a scalar") {
        Color c = Color::Color(0.2, 0.3, 0.4);
        REQUIRE(((c * 2) == Color::Color(0.4, 0.6, 0.8)));
    }

    SECTION("Multiplying colors") {
        Color c1 = Color::Color(1, 0.2, 0.4);
        Color c2 = Color::Color(0.9, 1, 0.1);
        REQUIRE((c1 * c2 == Color::Color(0.9, 0.2, 0.04)));
    }
}