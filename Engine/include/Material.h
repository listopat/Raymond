#pragma once
#include <Color.h>
#include <Lights/AreaLight.h>
#include <Lights/Light.h>
#include <Texture/Texture.h>

class Material {
public:
    std::shared_ptr<Texture> texture;
    Color color;
    double ambient, diffuse, specular, shininess;

    Material();
    Material(const Color &c, double a, double d, double sp, double sh);

    bool operator==(const Material &other) const;

    Color lighting(const Matrix &objectInverseTransform,
                   const Light &light,
                   const Tuple &point,
                   const Tuple &eyev,
                   const Tuple &nv,
                   double intensity) const;
};