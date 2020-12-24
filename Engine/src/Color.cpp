#include <Color.h>
#include <cmath>

static const double EPSILON = 0.0001;

Color::Color() : red(0), green(0), blue(0) {};
Color::Color(double r, double g, double b) : red(r), green(g), blue(b) {}
const Color Color::black = Color(0, 0, 0);
const Color Color::white = Color(1, 1, 1);

bool Color::operator==(const Color & other) const
{
    return fabs(red - other.red) < EPSILON
        && fabs(green - other.green) < EPSILON
        && fabs(blue - other.blue) < EPSILON;
}

Color Color::operator+(const Color & other) const {
    return Color(red + other.red, green + other.green, blue + other.blue);
}

Color Color::operator*(double scalar) const
{
    return Color(red * scalar, green * scalar, blue * scalar);
}

Color Color::operator*(const Color & other) const
{
    return Color(red * other.red, green * other.green, blue * other.blue);
}

Color Color::operator/(double scalar) const
{
    return Color(red / scalar, green / scalar, blue / scalar);
}

Color Color::operator-(const Color & other) const {
    return Color(red - other.red, green - other.green, blue - other.blue);
}

Color Color::operator-() const {
    return Color(-red, -green, -blue);
}
