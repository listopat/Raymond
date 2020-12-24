#pragma once
#include <Color.h>
#include <Tuple.h>

class Light {
public:
    Color intensity;

    Light(const Color &intensity) : intensity(intensity) {};
    virtual Color lighting(const Tuple & point, Color &color, double ambient, double diffuse, double specular, double shininess, double visibility, const Tuple & eyev, const Tuple & nv) const = 0;
};