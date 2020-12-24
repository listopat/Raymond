#pragma once
#include <Texture/Pattern.h>

class Stripe : public Pattern , public std::enable_shared_from_this<Stripe> {
public:
    Color a, b;

    Stripe(const Color &s1, const Color &s2);
    static std::shared_ptr<Stripe> createStripe(const Color &s1, const Color &s2);

    Color atUV(const UV &uv) const override;
};