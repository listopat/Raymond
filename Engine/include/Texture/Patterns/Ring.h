#pragma once
#include <Texture/Pattern.h>

class Ring : public Pattern, public std::enable_shared_from_this<Ring> {
public:
    Color a, b;

    Ring(const Color &s1, const Color &s2);
    static std::shared_ptr<Ring> createRing(const Color &s1, const Color &s2);

    Color atUV(const UV &uv) const override;
};
