#pragma once
#include <Lights/Light.h>

class AreaLight : public Light {
public:
    Tuple lowerLeftCorner, uVec, vVec, center;
    int uSteps, vSteps, samples;
    bool jitter;

    AreaLight(const Color &c, const Tuple &corner, const Tuple &fullUVec, int usteps, const Tuple &fullVVec, int vsteps, bool sampleJitter);
    
    Tuple sample(int u, int v) const;
    Color lighting(const Tuple & point, Color &color, double ambient, double diffuse, double specular, double shininess, double visibility, const Tuple & eyev, const Tuple & nv) const override;
};