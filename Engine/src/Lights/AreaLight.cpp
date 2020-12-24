#include <Lights/AreaLight.h>

AreaLight::AreaLight(const Color & c, const Tuple & corner, const Tuple & fullUVec, int usteps, const Tuple & fullVVec, int vsteps, bool sampleJitter) :
    Light(c),
    lowerLeftCorner(corner),
    uVec(fullUVec / usteps),
    uSteps(usteps),
    vVec(fullVVec / vsteps),
    vSteps(vsteps),
    samples(uSteps * vSteps),
    center(corner + fullUVec / 2 + fullVVec / 2),
    jitter(sampleJitter)
{
}

Tuple AreaLight::sample(int u, int v) const
{
    double uOffset = 0.5;
    double vOffset = 0.5;
    
    if (jitter) {
        uOffset = ((double)rand()) / RAND_MAX;
        vOffset = ((double)rand()) / RAND_MAX;
    }

    return lowerLeftCorner + uVec * (u + uOffset) + vVec * (v + vOffset);
}

Color AreaLight::lighting(const Tuple & point, Color & color, double ambient, double diffuse, double specular, double shininess, double visibility, const Tuple & eyev, const Tuple & nv) const
{
    Color ambientComponent = color * ambient;
    Color diffuseComponent = Color::Color(0, 0, 0);
    Color specularComponent = Color::Color(0, 0, 0);

    if (visibility > 0) {
        for (int u = 0; u < uSteps; u++) {
            for (int v = 0; v < vSteps; v++) {

                Tuple lightPosition = sample(u, v);
                Tuple lightv = Tuple::normalize(lightPosition - point);

                double lightDotNormal = Tuple::dot(lightv, nv);
                if (lightDotNormal >= 0.0) {
                    diffuseComponent = diffuseComponent + (color * diffuse * lightDotNormal);

                    Tuple reflectv = Tuple::reflect(-lightv, nv);
                    double reflectDotEye = Tuple::dot(reflectv, eyev);
                    if (reflectDotEye > 0) {
                        double factor = pow(reflectDotEye, shininess);
                        specularComponent = specularComponent + (intensity * specular * factor);
                    }
                }
            }
        }
    }

    return ambientComponent + (diffuseComponent + specularComponent) / samples * visibility;
}
