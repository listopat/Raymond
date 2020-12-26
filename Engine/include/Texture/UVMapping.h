#pragma once
#include <Tuple.h>
#include <memory>

struct UV {
    double u;
    double v;

    bool operator==(const UV& other) const;
};

class UVMapping {
public:
    UVMapping() {};
    virtual UV operator() (const Tuple &point) = 0;
};

class SphericalMap : public UVMapping, public std::enable_shared_from_this<SphericalMap> {
public:
    SphericalMap() {};
    static std::shared_ptr<SphericalMap> createSphericalMap();
    virtual UV operator() (const Tuple &point);
};

class PlanarMap : public UVMapping, public std::enable_shared_from_this<PlanarMap> {
public:
    PlanarMap() {};
    static std::shared_ptr<PlanarMap> createPlanarMap();
    virtual UV operator() (const Tuple &point);
};

class CubicMap : public UVMapping, public std::enable_shared_from_this<CubicMap> {
public:
    enum FACE {
        LEFT, RIGHT, FRONT, BACK, UP, DOWN
    };

    CubicMap() {};
    static std::shared_ptr<CubicMap> createCubicMap();
    virtual UV operator() (const Tuple& point);
    CubicMap::FACE faceFromPoint(const Tuple& point);
    UV UVFront(const Tuple& point);
    UV UVBack(const Tuple& point);
    UV UVLeft(const Tuple& point);
    UV UVRight(const Tuple& point);
    UV UVUp(const Tuple& point);
    UV UVDown(const Tuple& point);
};