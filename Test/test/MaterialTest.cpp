#include <catch.hpp>
#include <World.h>
#include <Material.h>
#include <Shapes/Sphere.h>

TEST_CASE("Lighting working properly", "[material]") {
    Material m = Material::Material();
    Tuple position = Tuple::point(0, 0, 0);

    SECTION("The default material") {
        Material m = Material::Material();
        REQUIRE((m.color == Color::Color(1.0, 1.0, 1.0)));
        REQUIRE(m.ambient == 0.1);
        REQUIRE(m.diffuse == 0.9);
        REQUIRE(m.specular == 0.9);
        REQUIRE(m.shininess == 200.0);
    }

    SECTION("Lighting with the eye between the light and the surface") {
        Tuple eyev = Tuple::vector(0, 0, -1);
        Tuple normalv = Tuple::vector(0, 0, -1);
        PointLight light = PointLight::PointLight(Tuple::point(0, 0, -10), Color::Color(1, 1, 1));
        Color result = m.lighting(Matrix::getIdentity4x4(), light, position, eyev, normalv, 1.0);
        REQUIRE((result == Color::Color(1.9, 1.9, 1.9)));
    }

    SECTION("Lighting with the eye between the light and the surface, eye offset 45") {
        Tuple eyev = Tuple::vector(0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0);
        Tuple normalv = Tuple::vector(0, 0, -1);
        PointLight light = PointLight::PointLight(Tuple::point(0, 0, -10), Color::Color(1, 1, 1));
        Color result = m.lighting(Matrix::getIdentity4x4(), light, position, eyev, normalv, 1.0);
        REQUIRE((result == Color::Color(1.0, 1.0, 1.0)));
    }

    SECTION("Lighting with the eye opposite surface, light offset 45") {
        Tuple eyev = Tuple::vector(0, 0, -1);
        Tuple normalv = Tuple::vector(0, 0, -1);
        PointLight light = PointLight::PointLight(Tuple::point(0, 10, -10), Color::Color(1, 1, 1));
        Color result = m.lighting(Matrix::getIdentity4x4(), light, position, eyev, normalv, 1.0);
        REQUIRE((result == Color::Color(0.7364, 0.7364, 0.7364)));
    }

    SECTION("Lighting with the eye in the path of the reflection vector") {
        Tuple eyev = Tuple::vector(0, -sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0);
        Tuple normalv = Tuple::vector(0, 0, -1);
        PointLight light = PointLight::PointLight(Tuple::point(0, 10, -10), Color::Color(1, 1, 1));
        Color result = m.lighting(Matrix::getIdentity4x4(), light, position, eyev, normalv, 1.0);
        REQUIRE((result == Color::Color(1.6364, 1.6364, 1.6364)));
    }

    SECTION("Lighting with the light behind the surface") {
        Tuple eyev = Tuple::vector(0, 0, -1);
        Tuple normalv = Tuple::vector(0, 0, -1);
        PointLight light = PointLight::PointLight(Tuple::point(0, 0, 10), Color::Color(1, 1, 1));
        Color result = m.lighting(Matrix::getIdentity4x4(), light, position, eyev, normalv, 1.0);
        REQUIRE((result == Color::Color(0.1, 0.1, 0.1)));
    }

    SECTION("Lighting with the surface in shadow") {
        Tuple eyev = Tuple::vector(0, 0, -1);
        Tuple normalv = Tuple::vector(0, 0, -1);
        PointLight light = PointLight::PointLight(Tuple::point(0, 0, -10), Color::Color(1, 1, 1));
        Color result = m.lighting(Matrix::getIdentity4x4(), light, position, eyev, normalv, 0.0);
        REQUIRE((result == Color::Color(0.1, 0.1, 0.1)));
    }

    SECTION("lighting() uses light intensity to attenuate color") {
        World w = World::makeDefaultWorld();
        w.getPointLight(0).intensity = Color(1, 1, 1);
        w.getPointLight(0).position = Tuple::point(0, 0, -10);
        w.getObject(0)->getMaterial().ambient = 0.1;
        w.getObject(0)->getMaterial().diffuse = 0.9;
        w.getObject(0)->getMaterial().specular = 0;
        w.getObject(0)->getMaterial().color = Color(1, 1, 1);
        Tuple pt = Tuple::point(0, 0, -1);
        Tuple eyev = Tuple::vector(0, 0, -1);
        Tuple normalv = Tuple::vector(0, 0, -1);
        Color result = w.getObject(0)->getMaterial().lighting(Matrix::getIdentity4x4(), w.getPointLight(0), pt, eyev, normalv, 1.0);
        REQUIRE((result == Color::Color(1, 1, 1)));
        result = w.getObject(0)->getMaterial().lighting(Matrix::getIdentity4x4(), w.getPointLight(0), pt, eyev, normalv, 0.5);
        REQUIRE((result == Color::Color(0.55, 0.55, 0.55)));
        result = w.getObject(0)->getMaterial().lighting(Matrix::getIdentity4x4(), w.getPointLight(0), pt, eyev, normalv, 0.0);
        REQUIRE((result == Color::Color(0.1, 0.1, 0.1)));
    }

    SECTION("lighting() samples the area light") {
        Tuple corner = Tuple::point(-0.5, -0.5, -5);
        Color intensity = Color::Color(1, 1, 1);
        Tuple v1 = Tuple::vector(1, 0, 0);
        Tuple v2 = Tuple::vector(0, 1, 0);
        AreaLight light = AreaLight(intensity, corner, v1, 2, v2, 2, false);

        std::shared_ptr<Sphere> s1 = Sphere::createSphere();
        s1->setMaterial(Material(Color(1, 1, 1), 0.1, 0.9, 0, 200, 0, 0, 1));

        Tuple eye = Tuple::point(0, 0, 5);
        
        Tuple pt = Tuple::point(0, 0, -1);
        Tuple eyev = Tuple::normalize(eye - pt);
        Tuple normalv = Tuple::vector(pt[0], pt[1], pt[2]);
        Color result = s1->getMaterial().lighting(Matrix::getIdentity4x4(), light, pt, eyev, normalv, 1.0);
        REQUIRE(result == Color(0.9965, 0.9965, 0.9965));

        pt = Tuple::point(0, 0.7071, -0.7071);
        eyev = Tuple::normalize(eye - pt);
        normalv = Tuple::vector(pt[0], pt[1], pt[2]);
        result = s1->getMaterial().lighting(Matrix::getIdentity4x4(), light, pt, eyev, normalv, 1.0);
        REQUIRE(result == Color(0.6232, 0.6232, 0.6232));
    }

    SECTION("Reflectivity for the default material") {
        REQUIRE(m.reflective == 0.0);
    }

    SECTION("Transparency and Refractive Index for the default material") {
        REQUIRE(m.transparency == 0.0);
        REQUIRE(m.refractiveIndex == 1.0);
    }
}