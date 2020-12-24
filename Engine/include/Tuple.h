#pragma once
#include <array>

class Tuple {
public:
    Tuple(double x, double y, double z, double w);

    static Tuple point(double x, double y, double z);
    static Tuple vector(double x, double y, double z);
    static bool isPoint(const Tuple &tuple);
    static bool isVector(const Tuple &tuple);
    
    double& operator[] (int x);
    double operator[] (int x) const;
    bool operator==(const Tuple &other) const;
    Tuple operator+(const Tuple &other) const;
    Tuple operator*(double scalar) const;
    Tuple operator/(double scalar) const;
    Tuple operator-(const Tuple &other) const;
    Tuple operator-() const;

    static double magnitude(const Tuple &tuple);
    static Tuple normalize(const Tuple &tuple);
    static double dot(const Tuple &t1, const Tuple &t2);
    static Tuple cross(const Tuple &t1, const Tuple &t2);
    static Tuple reflect(const Tuple &in, const Tuple &normal);

private:
    std::array<double, 4> values;
};
