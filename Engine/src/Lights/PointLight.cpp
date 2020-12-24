#include <Lights/PointLight.h>

PointLight::PointLight() : position(Tuple::point(0, 0, 0)) , Light(Color(0, 0, 0))
{
}

PointLight::PointLight(const Tuple & p, const Color & c) : position(p) , Light(c)
{
}

Color PointLight::lighting(const Tuple & point, Color &color, double ambient, double diffuse, double specular, double shininess, double visibility, const Tuple & eyev, const Tuple & nv) const
{
    Color ambientComponent = color * ambient;
    Color diffuseComponent = Color::Color(0, 0, 0);
    Color specularComponent = Color::Color(0, 0, 0);

    Tuple lightv = Tuple::normalize(position - point);

    if (visibility > 0) {
        double lightDotNormal = Tuple::dot(lightv, nv);
        if (lightDotNormal >= 0.0) {
            diffuseComponent = color * diffuse * lightDotNormal;

            Tuple reflectv = Tuple::reflect(-lightv, nv);
            double reflectDotEye = Tuple::dot(reflectv, eyev);
            if (reflectDotEye > 0) {
                double factor = pow(reflectDotEye, shininess);
                specularComponent = intensity * specular * factor;
            }
        }
    }

    return ambientComponent + (diffuseComponent + specularComponent) * visibility;
}
