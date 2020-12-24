#include <Camera.h>
#include <math.h>
#include <iostream>

Camera Camera::makeCamera(int hSize, int vSize, double fov)
{
    double halfView = tan(fov / 2);
    double aspectRatio = ((double) hSize) / vSize;
    
    double halfWidth, halfHeight;
    if (aspectRatio >= 1) {
        halfWidth = halfView;
        halfHeight = halfView / aspectRatio;
    }
    else {
        halfWidth = halfView * aspectRatio;
        halfHeight = halfView;
    }
    double pixelSize = (halfWidth * 2) / hSize;
    return Camera(hSize, vSize, fov, pixelSize, Matrix::getIdentity4x4(), halfWidth, halfHeight);
}

void Camera::setTransform(const Matrix& m)
{
    transform = m;
    inverseTransform = Matrix::inverse(transform);
}

Matrix Camera::getTransform() const
{
    return transform;
}

Ray Camera::rayForPixel(int px, int py)
{
    double xOffset = (px + 0.5) * pixelSize;
    double yOffset = (py + 0.5) * pixelSize;

    double worldX = halfWidth - xOffset;
    double worldY = halfHeight - yOffset;

    Tuple pixel = inverseTransform * Tuple::point(worldX, worldY, -1);
    Tuple origin = inverseTransform * Tuple::point(0, 0, 0);
    Tuple direction = Tuple::normalize(pixel - origin);

    return Ray(origin, direction);
}

Canvas Camera::render(World w)
{
    Canvas image = Canvas(hSize, vSize);
    for (int y = 0; y < vSize; y++) {
        for (int x = 0; x < hSize; x++) {
            Ray ray = rayForPixel(x, y);
            Color color = w.colorAt(ray);
            image.set(x, y, color);
        }
        std::cout << "Rendered row " << y << std::endl;
    }
    return image;
}

Camera::Camera(int hSize, int vSize, double fov, double pixelSize, const Matrix &m, double halfWidth, double halfHeight) : hSize(hSize), vSize(vSize), fieldOfView(fov),
pixelSize(pixelSize), transform(m), inverseTransform(Matrix::inverse(m)), halfWidth(halfWidth), halfHeight(halfHeight)
{
}