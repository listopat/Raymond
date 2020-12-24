#include <Texture/Patterns/Stripe.h>
#include <math.h>

Stripe::Stripe(const Color &s1, const Color &s2) : a(s1), b(s2) {}

std::shared_ptr<Stripe> Stripe::createStripe(const Color &s1, const Color &s2)
{
    return std::make_shared<Stripe>(Stripe(s1, s2));
}

Color Stripe::atUV(const UV &uv) const
{
    return ((int)floor(2 * uv.u)) % 2 == 0 ? a : b;
}