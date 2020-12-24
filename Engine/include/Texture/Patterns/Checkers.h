#pragma once
#include <Texture/Pattern.h>

class Checkers : public Pattern, public std::enable_shared_from_this<Checkers> {
public:
    Color a, b;
    int width, height;

    Checkers(const int width, const int height, const Color &s1, const Color &s2);
    static std::shared_ptr<Checkers> createCheckers(const int width, const int height, const Color &s1, const Color &s2);

    Color atUV(const UV &uv) const override;
};