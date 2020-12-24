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