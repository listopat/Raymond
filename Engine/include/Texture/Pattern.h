#pragma once
#include <Color.h>
#include <Texture/UVMapping.h>

class Pattern {
public:
    Color intensity;
    virtual Color atUV(const UV &uv) const = 0;
};