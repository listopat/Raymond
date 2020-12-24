#pragma once
class Color {
public:
    double red, green, blue;

    Color();
    Color(double r, double g, double b);
    static const Color black;
    static const Color white;

    bool operator==(const Color &other) const;
    Color operator+(const Color &other) const;
    Color operator*(double scalar) const;
    Color operator*(const Color &other) const;
    Color operator/(double scalar) const;
    Color operator-(const Color &other) const;
    Color operator-() const;
};
