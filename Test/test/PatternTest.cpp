#include <catch.hpp>
#include <Color.h>
#include <Texture/Patterns/Stripe.h>
#include <Texture/Patterns/Gradient.h>
#include <Texture/Patterns/Ring.h>
#include <Texture/Patterns/Checkers.h>
#include <Texture/Texture.h>
#include <Shapes/Sphere.h>
#include <PPMInterface.h>
#include <Texture/UVImage.h>

TEST_CASE("Mappings working as expected", "[mapping]") {
    SECTION("Using a spherical mapping on a 3D point") {
        SphericalMap map = SphericalMap();
        REQUIRE(map(Tuple::point(0, 0, -1)) == UV{ 0, 0.5 });
        REQUIRE(map(Tuple::point(1, 0, 0)) == UV{ 0.25, 0.5 });
        REQUIRE(map(Tuple::point(0, 0, 1)) == UV{ 0.5, 0.5 });
        REQUIRE(map(Tuple::point(-1, 0, 0)) == UV{ 0.75, 0.5 });
        REQUIRE(map(Tuple::point(0, 1, 0)) == UV{ 0.5, 1.0 });
        REQUIRE(map(Tuple::point(0, -1, 0)) == UV{ 0.5, 0.0 });
        REQUIRE(map(Tuple::point(sqrt(2) / 2, sqrt(2) / 2, 0)) == UV{ 0.25, 0.75 });
    }

    SECTION("Using a planar mapping on a 3D point") {
        PlanarMap map = PlanarMap();
        REQUIRE(map(Tuple::point(0.25, 0, 0.5)) == UV{ 0.25, 0.5 });
        REQUIRE(map(Tuple::point(0.25, 0, -0.25)) == UV{ 0.25, 0.75 });
        REQUIRE(map(Tuple::point(0.25, 0, -0.25)) == UV{ 0.25, 0.75 });
        REQUIRE(map(Tuple::point(1.25, 0, 0.5)) == UV{ 0.25, 0.5 });
        REQUIRE(map(Tuple::point(0.25, 0, -1.75)) == UV{ 0.25, 0.25 });
        REQUIRE(map(Tuple::point(1, 0, -1)) == UV{ 0, 0 });
        REQUIRE(map(Tuple::point(0, 0, 0)) == UV{ 0, 0 });
    }
}

TEST_CASE("Patterns working as expected", "[pattern]") {
    SECTION("A gradient linearly interpolates between colors") {
        Stripe pattern = Stripe(Color::white, Color::black);
        REQUIRE(pattern.atUV(UV{ 0, 0 }) == Color::white);
        REQUIRE(pattern.atUV(UV{ 0.4, 0 }) == Color::white);
        REQUIRE(pattern.atUV(UV{ 0.8, 0 }) == Color::black);
        REQUIRE(pattern.atUV(UV{ 0.9, 0 }) == Color::black);
    }

    SECTION("A gradient linearly interpolates between colors") {
        Gradient pattern = Gradient(Color::white, Color::black);
        REQUIRE(pattern.atUV(UV{0, 0}) == Color::white);
        REQUIRE(pattern.atUV(UV{ 0.25, 0 }) == Color(0.75, 0.75, 0.75));
        REQUIRE(pattern.atUV(UV{ 0.5, 0 }) == Color(0.5, 0.5, 0.5));
        REQUIRE(pattern.atUV(UV{ 0.75, 0 }) == Color(0.25, 0.25, 0.25));
    }

    SECTION("Checker pattern in 2D") {
        Checkers pattern = Checkers(2, 2, Color::black, Color::white);
        REQUIRE(pattern.atUV(UV{ 0, 0 }) == Color::black);
        REQUIRE(pattern.atUV(UV{ 0.5, 0 }) == Color::white);
        REQUIRE(pattern.atUV(UV{ 0.0, 0.5 }) == Color::white);
        REQUIRE(pattern.atUV(UV{ 0.5, 0.5 }) == Color::black);
        REQUIRE(pattern.atUV(UV{ 1.0, 1.0 }) == Color::black);
    }

    SECTION("A ring should extend in both x and z") {
        Ring pattern = Ring(Color::white, Color::black);
        REQUIRE(pattern.atUV(UV{ 0, 0 }) == Color::white);
        REQUIRE(pattern.atUV(UV{ 1, 0 }) == Color::black);
        REQUIRE(pattern.atUV(UV{ 0, 1 }) == Color::black);
        REQUIRE(pattern.atUV(UV{ 0.708, 0.708 }) == Color::black);
    }
}

TEST_CASE("Texture", "[texture]") {
    SECTION("Pattern with an object transformation") {
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        s->setTransform(Matrix::scaling(2, 2, 2));

        std::shared_ptr<Stripe> pattern = Stripe::createStripe(Color::white, Color::black);
        std::shared_ptr<SphericalMap> map = SphericalMap::createSphericalMap();

        Texture texture = Texture(pattern, map);
        Color c = texture.atObject(s->getInverseTransform(), Tuple::point(1.5, 0, 0));
        REQUIRE(c == Color::white);
    }

    SECTION("Pattern with a pattern transformation") {
        std::shared_ptr<Sphere> s = Sphere::createSphere();

        std::shared_ptr<Stripe> pattern = Stripe::createStripe(Color::white, Color::black);
        std::shared_ptr<SphericalMap> map = SphericalMap::createSphericalMap();

        Texture texture = Texture(pattern, map);
        texture.setTransform(Matrix::scaling(2, 2, 2));
        Color c = texture.atObject(s->getInverseTransform(), Tuple::point(1.5, 0, 0));
        REQUIRE(c == Color::white);
    }

    SECTION("Pattern with both an object and a pattern transformation") {
        std::shared_ptr<Sphere> s = Sphere::createSphere();
        s->setTransform(Matrix::scaling(2, 2, 2));
        std::shared_ptr<Stripe> pattern = Stripe::createStripe(Color::white, Color::black);
        std::shared_ptr<SphericalMap> map = SphericalMap::createSphericalMap();

        Texture texture = Texture(pattern, map);
        texture.setTransform(Matrix::scaling(2, 2, 2));
        Color c = texture.atObject(s->getInverseTransform(), Tuple::point(2.5, 0, 0));
        REQUIRE(c == Color::white);
    }

    SECTION("Mapping checkers to a spherical map") {
        std::shared_ptr<Checkers> checkers = Checkers::createCheckers(16, 8, Color::black, Color::white);
        std::shared_ptr<SphericalMap> map = SphericalMap::createSphericalMap();
        Texture texture = Texture(checkers, map);
        REQUIRE(texture.atObject(Matrix::getIdentity4x4(), Tuple::point(0.4315, 0.4670, 0.7719)) == Color::white);
;    }

    SECTION("Grayscale patter in 2D from image") {
        std::string ppm = "";
        ppm.append("P3\n");
        ppm.append("10 10\n");
        ppm.append("10\n");
        ppm.append("0 0 0  1 1 1  2 2 2  3 3 3  4 4 4  5 5 5  6 6 6  7 7 7  8 8 8  9 9 9\n");
        ppm.append("1 1 1  2 2 2  3 3 3  4 4 4  5 5 5  6 6 6  7 7 7  8 8 8  9 9 9  0 0 0\n");
        ppm.append("2 2 2  3 3 3  4 4 4  5 5 5  6 6 6  7 7 7  8 8 8  9 9 9  0 0 0  1 1 1\n");
        ppm.append("3 3 3  4 4 4  5 5 5  6 6 6  7 7 7  8 8 8  9 9 9  0 0 0  1 1 1  2 2 2\n");
        ppm.append("4 4 4  5 5 5  6 6 6  7 7 7  8 8 8  9 9 9  0 0 0  1 1 1  2 2 2  3 3 3\n");
        ppm.append("5 5 5  6 6 6  7 7 7  8 8 8  9 9 9  0 0 0  1 1 1  2 2 2  3 3 3  4 4 4\n");
        ppm.append("6 6 6  7 7 7  8 8 8  9 9 9  0 0 0  1 1 1  2 2 2  3 3 3  4 4 4  5 5 5\n");
        ppm.append("7 7 7  8 8 8  9 9 9  0 0 0  1 1 1  2 2 2  3 3 3  4 4 4  5 5 5  6 6 6\n");
        ppm.append("8 8 8  9 9 9  0 0 0  1 1 1  2 2 2  3 3 3  4 4 4  5 5 5  6 6 6  7 7 7\n");
        ppm.append("9 9 9  0 0 0  1 1 1  2 2 2  3 3 3  4 4 4  5 5 5  6 6 6  7 7 7  8 8 8\n");
        std::shared_ptr<Canvas> canvas = std::make_shared<Canvas>(PPMInterface::canvasFromPPM(ppm));
        UVImage image = UVImage(canvas);
        REQUIRE(image.atUV(UV{ 0, 0 }) == Color(0.9, 0.9, 0.9));
        REQUIRE(image.atUV(UV{ 0.3, 0 }) == Color(0.2, 0.2, 0.2));
        REQUIRE(image.atUV(UV{ 0.6, 0.3 }) == Color(0.1, 0.1, 0.1));
        REQUIRE(image.atUV(UV{ 1, 1 }) == Color(0.9, 0.9, 0.9));
    }
}
