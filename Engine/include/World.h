#pragma once
#include <Lights/PointLight.h>
#include <Lights/AreaLight.h>
#include <Shapes/Shape.h>
#include <Intersection.h>
#include <Hit.h>
#include <vector>

class World {
public:
    World();
    static World makeDefaultWorld();

    int getLightCount() const;
    void addLight(const PointLight &pointLight);
    void addLight(const AreaLight &areaLight);
    PointLight& getPointLight(int index);
    PointLight getPointLight(int index) const;
    AreaLight& getAreaLight(int index);
    AreaLight getAreaLight(int index) const;

    int getObjectCount() const;
    void addObject(const std::shared_ptr<Shape> &object);
    std::shared_ptr<Shape> getObject(int index) const;

    std::vector<Intersection> intersects(const Ray &r) const;
    Color shadeHit(Hit hit) const;
    bool isShadowed(const Tuple &lightPosition, const Tuple &point) const;
    double intensityAt(const PointLight &light, const Tuple &point) const;
    double intensityAt(const AreaLight &light, const Tuple &point) const;
    Color colorAt(const Ray &r) const;

private:
    std::vector<PointLight> pointLights;
    std::vector<AreaLight> areaLights;
    std::vector<std::shared_ptr<Shape>> objects;
};