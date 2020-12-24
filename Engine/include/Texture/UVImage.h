#pragma once
#include <Texture/Pattern.h>
#include <Canvas.h>

class UVImage : public Pattern, public std::enable_shared_from_this<UVImage> {
public:
    std::shared_ptr<Canvas> image;

    UVImage(std::shared_ptr<Canvas> canvas);
    static std::shared_ptr<UVImage> createUVImage(std::shared_ptr<Canvas> canvas);

    Color atUV(const UV &uv) const override;
};