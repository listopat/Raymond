#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>
#include <Tuple.h>

TEST_CASE("Tuples are constructed correctly.", "[tuple]") {
    SECTION("A Tuple with w = 1.0 is a point") {
        Tuple t = Tuple(4.3, -4.2, 3.1, 1.0);
        REQUIRE(t[0] == 4.3);
        REQUIRE(t[1] == -4.2);
        REQUIRE(t[2] == 3.1);
        REQUIRE(t[3] == 1.0);

        REQUIRE(Tuple::isPoint(t));
        REQUIRE(!Tuple::isVector(t));
    }

    SECTION("A Tuple with w = 0.0 is a vector") {
        Tuple t = Tuple(4.3, -4.2, 3.1, 0.0);
        REQUIRE(t[0] == 4.3);
        REQUIRE(t[1] == -4.2);
        REQUIRE(t[2] == 3.1);
        REQUIRE(t[3] == 0.0);
        REQUIRE(Tuple::isVector(t));
        REQUIRE(!Tuple::isPoint(t));
    }

    SECTION("Equality operator on tuples returns true when their values are equal") {
        Tuple t1 = Tuple(4.0, -4.0, 3.0, 1.0);
        Tuple t2 = Tuple(4.f, -4.0, 3.0, 1.0);
        REQUIRE((t1 == t2));
    }

    SECTION("point() creates tuples with w = 1.0") {
        Tuple t1 = Tuple::point(4.0, -4.0, 3.0);
        Tuple t2 = Tuple(4.0, -4.0, 3.0, 1.0);
        REQUIRE((t1 == t2));
    }

    SECTION("vector() creates tuples with w = 0.0") {
        Tuple t1 = Tuple::vector(4.0, -4.0, 3.0);
        Tuple t2 = Tuple(4.0, -4.0, 3.0, 0.0);
        REQUIRE((t1 == t2));
    }
}

TEST_CASE("Tuple operations are calculated properly.", "[tuple]") {

    static const double EPSILON = 0.00001;

    SECTION("Adding two tuples") {
        Tuple t1 = Tuple(3, -2, 5, 1);
        Tuple t2 = Tuple(-2, 3, 1, 0);
        Tuple t3 = Tuple(1, 1, 6, 1);
        REQUIRE(((t1 + t2) == t3));
    }

    SECTION("Subtracting two points") {
        Tuple t1 = Tuple::point(3, 2, 1);
        Tuple t2 = Tuple::point(5, 6, 7);
        Tuple t3 = Tuple::vector(2, 4, 6);
        REQUIRE(((t2 - t1) == t3));
    }

    SECTION("Subtracting a vector from a point") {
        Tuple t1 = Tuple::point(3, 2, 1);
        Tuple t2 = Tuple::vector(5, 6, 7);
        Tuple t3 = Tuple::point(-2, -4, -6);
        REQUIRE(((t1 - t2) == t3));
    }

    SECTION("Subtracting two vectors") {
        Tuple t1 = Tuple::vector(3, 2, 1);
        Tuple t2 = Tuple::vector(5, 6, 7);
        Tuple t3 = Tuple::vector(-2, -4, -6);
        REQUIRE(((t1 - t2) == t3));
    }

    SECTION("Negating a tuple") {
        Tuple t1 = Tuple(1, -2, 3, -4);
        Tuple t2 = Tuple(-1, 2, -3, 4);
        REQUIRE((-t1 == t2));
    }

    SECTION("Multiplying a tuple by a scalar") {
        Tuple t1 = Tuple(1, -2, 3, -4);
        Tuple t2 = Tuple(3.5, -7, 10.5, -14);
        REQUIRE(((t1 * 3.5) == t2));
    }

    SECTION("Multiplying a tuple by a fraction") {
        Tuple t1 = Tuple(1, -2, 3, -4);
        Tuple t2 = Tuple(0.5, -1, 1.5, -2);
        REQUIRE(((t1 * 0.5) == t2));
    }

    SECTION("Dividing a tuple by a scalar") {
        Tuple t1 = Tuple(1, -2, 3, -4);
        Tuple t2 = Tuple(0.5, -1, 1.5, -2);
        REQUIRE(((t1 / 2.0) == t2));
    }

    SECTION("Computing the magnitude of vector(1, 0, 0)") {
        Tuple t = Tuple::vector(1, 0, 0);
        REQUIRE(abs(Tuple::magnitude(t) - 1.0) < EPSILON);
    }

    SECTION("Computing the magnitude of vector(0, 1, 0)") {
        Tuple t = Tuple::vector(0, 1, 0);
        REQUIRE(abs(Tuple::magnitude(t) - 1.0) < EPSILON);
    }

    SECTION("Computing the magnitude of vector(0, 0, 1)") {
        Tuple t = Tuple::vector(0, 0, 1);
        REQUIRE(abs(Tuple::magnitude(t) - 1.0) < EPSILON);
    }

    SECTION("Computing the magnitude of vector(1, 2, 3)") {
        Tuple t = Tuple::vector(1, 2, 3);
        REQUIRE(abs(Tuple::magnitude(t) - sqrt(14.0)) < EPSILON);
    }

    SECTION("Computing the magnitude of vector(-1, -2, -3)") {
        Tuple t = Tuple::vector(-1, -2, -3);
        REQUIRE(abs(Tuple::magnitude(t) - sqrt(14.0)) < EPSILON);
    }

    SECTION("Normalizing vector(4, 0, 0) gives (1, 0, 0)") {
        Tuple t = Tuple::vector(4, 0, 0);
        REQUIRE((Tuple::normalize(t) == Tuple::vector(1, 0, 0)));
    }

    SECTION("Normalizing vector(1, 2, 3)") {
        Tuple t = Tuple::vector(1, 2, 3);
        REQUIRE((Tuple::normalize(t) == Tuple::vector(0.26726, 0.53452, 0.80178)));
    }

    SECTION("The magnitude of normalized vector") {
        Tuple t = Tuple::vector(1, 2, 3);
        REQUIRE((Tuple::magnitude(Tuple::normalize(t)) - 1.0) < EPSILON);
    }

    SECTION("The dot product of two tuples") {
        Tuple t1 = Tuple::vector(1, 2, 3);
        Tuple t2 = Tuple::vector(2, 3, 4);
        REQUIRE((Tuple::dot(t1, t2) - 20.0) < EPSILON);
    }

    SECTION("The cross product of two vect") {
        Tuple t1 = Tuple::vector(1, 2, 3);
        Tuple t2 = Tuple::vector(2, 3, 4);
        REQUIRE((Tuple::cross(t1, t2) == Tuple::vector(-1, 2, -1)));
        REQUIRE((Tuple::cross(t2, t1) == Tuple::vector(1, -2, 1)));
    }

    SECTION("Reflecting a vector approaching at 45 degrees") {
        Tuple v = Tuple::vector(1, -1, 0);
        Tuple n = Tuple::vector(0, 1, 0);
        Tuple r = Tuple::reflect(v, n);
        REQUIRE((r == Tuple::vector(1, 1, 0)));
    }

    SECTION("Reflecting a vector off a slanted surface") {
        Tuple v = Tuple::vector(0, -1, 0);
        Tuple n = Tuple::vector(sqrt(2.0) / 2.0, sqrt(2.0) / 2.0, 0);
        Tuple r = Tuple::reflect(v, n);
        REQUIRE((r == Tuple::vector(1, 0, 0)));
    }
}
