#pragma once
#include <Lights/Light.h>

class PointLight : public Light {
public:
    Tuple position;

    PointLight();
    PointLight(const Tuple &p, const Color &c);

    Color lighting(const Tuple & point, Color &color, double ambient, double diffuse, double specular, double shininess, double visibility, const Tuple & eyev, const Tuple & nv) const override;
};