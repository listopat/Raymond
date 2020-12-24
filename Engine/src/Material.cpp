#include <Material.h>

Material::Material() : color(Color::Color(1.0, 1.0, 1.0)) , ambient(0.1), diffuse(0.9), specular(0.9), shininess(200)
{
}

Material::Material(const Color &c, double a, double d, double sp, double sh) : color(c) , ambient(a) , diffuse(d) , specular(sp) , shininess(sh)
{
}

bool Material::operator==(const Material & other) const
{
    return color == other.color
        && ambient == other.ambient
        && diffuse == other.diffuse
        && specular == other.specular
        && shininess == other.shininess;
}

Color Material::lighting(const Matrix &objectInverseTransform, const Light &light, const Tuple & point, const Tuple & eyev, const Tuple & nv, double intensity) const
{
    Color materialColor = texture ? texture->atObject(objectInverseTransform, point) : color;
    Color effectiveColor = materialColor * light.intensity;

    return light.lighting(point, effectiveColor, ambient, diffuse, specular, shininess, intensity, eyev, nv);
}