#pragma once
#include <Matrix.h>
#include <Ray.h>
#include <Canvas.h>
#include <World.h>

class Camera {
public:
    const int hSize, vSize;
    const double fieldOfView, pixelSize;

    static Camera makeCamera(int h, int v, double fov);

    void setTransform(const Matrix& m);
    Matrix getTransform() const;

    Ray rayForPixel(int px, int py);
    Canvas render(World w);

private:
    Matrix transform, inverseTransform;
    const double halfWidth, halfHeight;
    Camera(int h, int v, double fov, double pxlSz, const Matrix &m, double halfWidth, double halfHeight);
};