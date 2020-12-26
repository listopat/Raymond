#include <Texture/UVImage.h>

UVImage::UVImage(std::shared_ptr<Canvas> canvas) : image(canvas) {}

std::shared_ptr<UVImage> UVImage::createUVImage(std::shared_ptr<Canvas> canvas)
{
    return std::make_shared<UVImage>(UVImage(canvas));
}

Color UVImage::atUV(const UV & uv) const
{
    double v = 1 - uv.v;
    double x = uv.u * (image->width - 1);
    double y = v * (image->height - 1);
    return image->at(static_cast<int>(round(x)), static_cast<int>(round(y)));
}
