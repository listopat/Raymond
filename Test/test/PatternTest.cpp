#include <catch.hpp>
#include <Color.h>
#include <Texture/Patterns/Stripe.h>
#include <Texture/Patterns/Gradient.h>
#include <Texture/Patterns/Ring.h>
#include <Texture/Patterns/Checkers.h>
#include <Texture/Texture.h>
#include <Shapes/Sphere.h>
#include <ImageIOInterface.h>
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

    SECTION("Identifying the face of a cube from a point") {
        CubicMap map = CubicMap();
        REQUIRE(map.faceFromPoint(Tuple::point(-1, 0.5, -0.25)) == CubicMap::FACE::LEFT);
        REQUIRE(map.faceFromPoint(Tuple::point(1.1, -0.75, 0.8)) == CubicMap::FACE::RIGHT);
        REQUIRE(map.faceFromPoint(Tuple::point(0.1, 0.6, 0.9)) == CubicMap::FACE::FRONT);
        REQUIRE(map.faceFromPoint(Tuple::point(-0.7, 0, -2)) == CubicMap::FACE::BACK);
        REQUIRE(map.faceFromPoint(Tuple::point(0.5, 1, 0.9)) == CubicMap::FACE::UP);
        REQUIRE(map.faceFromPoint(Tuple::point(-0.2, -1.3, 1.1)) == CubicMap::FACE::DOWN);
    }

    SECTION("UV mapping the front face of a cube") {
        CubicMap map = CubicMap();
        REQUIRE(map.UVFront(Tuple::point(-0.5, 0.5, 1)) == UV{ 0.25, 0.75 });
        REQUIRE(map.UVFront(Tuple::point(0.5, -0.5, 1)) == UV{ 0.75, 0.25 });
    }

    SECTION("UV mapping the back face of a cube") {
        CubicMap map = CubicMap();
        REQUIRE(map.UVBack(Tuple::point(0.5, 0.5, -1)) == UV{ 0.25, 0.75 });
        REQUIRE(map.UVBack(Tuple::point(-0.5, -0.5, -1)) == UV{ 0.75, 0.25 });
    }

    SECTION("UV mapping the left face of a cube") {
        CubicMap map = CubicMap();
        REQUIRE(map.UVLeft(Tuple::point(-1, 0.5, -0.5)) == UV{ 0.25, 0.75 });
        REQUIRE(map.UVLeft(Tuple::point(-1, -0.5, 0.5)) == UV{ 0.75, 0.25 });
    }

    SECTION("UV mapping the right face of a cube") {
        CubicMap map = CubicMap();
        REQUIRE(map.UVRight(Tuple::point(1, 0.5, 0.5)) == UV{ 0.25, 0.75 });
        REQUIRE(map.UVRight(Tuple::point(1, -0.5, -0.5)) == UV{ 0.75, 0.25 });
    }

    SECTION("UV mapping the upper face of a cube") {
        CubicMap map = CubicMap();
        REQUIRE(map.UVUp(Tuple::point(-0.5, 1, -0.5)) == UV{ 0.25, 0.75 });
        REQUIRE(map.UVUp(Tuple::point(0.5, 1, 0.5)) == UV{ 0.75, 0.25 });
    }

    SECTION("UV mapping the lower face of a cube") {
        CubicMap map = CubicMap();
        REQUIRE(map.UVDown(Tuple::point(-0.5, -1, 0.5)) == UV{ 0.25, 0.75 });
        REQUIRE(map.UVDown(Tuple::point(0.5, -1, -0.5)) == UV{ 0.75, 0.25 });
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
}
