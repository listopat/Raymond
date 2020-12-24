#include <Texture/Texture.h>

Texture::Texture(std::shared_ptr<Pattern> texture, std::shared_ptr<UVMapping> mapping) :
    texture(texture), 
    mapping(mapping),
    transform(Matrix::getIdentity4x4()), 
    inverseTransform(Matrix::getIdentity4x4())
{
}

std::shared_ptr<Texture> Texture::createTexture(std::shared_ptr<Pattern> texture, std::shared_ptr<UVMapping> mapping)
{
    return std::make_shared<Texture>(Texture(texture, mapping));
}

void Texture::setTransform(const Matrix & m)
{
    transform = m;
    inverseTransform = Matrix::inverse(transform);
}

Matrix Texture::getTransform() const
{
    return transform;;
}

Matrix Texture::getInverseTransform() const
{
    return inverseTransform;
}

Color Texture::atObject(const Matrix &inverseObjectTransform, const Tuple &point) const
{
    Tuple objectPoint = inverseObjectTransform * point;
    Tuple texturePoint = inverseTransform * objectPoint;
    return texture->atUV(mapping->operator()(texturePoint));
}