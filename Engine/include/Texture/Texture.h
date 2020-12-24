#pragma once
#include <Matrix.h>
#include <Color.h>
#include <Texture/Pattern.h>
#include <Texture/UVMapping.h>

class Texture : public std::enable_shared_from_this<Texture> {
public:
    std::shared_ptr<Pattern> texture;
    std::shared_ptr<UVMapping> mapping;

    Texture(std::shared_ptr<Pattern> texture, std::shared_ptr<UVMapping> mapping);
    static std::shared_ptr<Texture> createTexture(std::shared_ptr<Pattern> texture, std::shared_ptr<UVMapping> mapping);

    void setTransform(const Matrix &m);
    Matrix getTransform() const;
    Matrix getInverseTransform() const;

    Color atObject(const Matrix &inverseObjectTransform, const Tuple &point) const;

private:
    Matrix transform;
    Matrix inverseTransform;
};