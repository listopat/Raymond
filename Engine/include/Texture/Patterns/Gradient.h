#pragma once
#include <Texture/Pattern.h>

class Gradient : public Pattern , public std::enable_shared_from_this<Gradient> {
public:
    Color a, b;

    Gradient(const Color &s1, const Color &s2);
    static std::shared_ptr<Gradient> createGradient(const Color &s1, const Color &s2);

    Color atUV(const UV &uv) const override;
};