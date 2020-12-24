#include <Tuple.h>

static const double EPSILON = 0.00001;

Tuple::Tuple(double x, double y, double z, double w) : values({ x, y, z, w }) {}

Tuple Tuple::point(double x, double y, double z) {
    return Tuple(x, y, z, 1.0);
}

Tuple Tuple::vector(double x, double y, double z) {
    return Tuple(x, y, z, 0.0);
}

bool Tuple::isPoint(const Tuple &tuple) {
    return fabs(tuple.values[3] - 1.0) < EPSILON;
}

bool Tuple::isVector(const Tuple &tuple) {
    return fabs(tuple.values[3] - 0.0) < EPSILON;
}

double& Tuple::operator[] (int x) {
    return values[x];
}

double Tuple::operator[](int x) const
{
    return values[x];
}

bool Tuple::operator==(const Tuple & other) const
{
    return abs(values[0] - other.values[0]) < EPSILON
        && abs(values[1] - other.values[1]) < EPSILON
        && abs(values[2] - other.values[2]) < EPSILON
        && abs(values[3] - other.values[3]) < EPSILON;
}

Tuple Tuple::operator+(const Tuple & other) const
{
    return Tuple(values[0] + other.values[0],
        values[1] + other.values[1],
        values[2] + other.values[2],
        values[3] + other.values[3]);
}

Tuple Tuple::operator*(double scalar) const
{
    return Tuple(values[0] * scalar, values[1] * scalar, values[2] * scalar, values[3] * scalar);
}

Tuple Tuple::operator/(double scalar) const {
    return Tuple(values[0] / scalar, values[1] / scalar, values[2] / scalar, values[3] / scalar);
}

Tuple Tuple::operator-(const Tuple & other) const
{
    return Tuple(values[0] - other.values[0],
                values[1] - other.values[1],
                values[2] - other.values[2],
                values[3] - other.values[3]);
}

Tuple Tuple::operator-() const
{
    return Tuple(-values[0], -values[1], -values[2], -values[3]);
}

double Tuple::magnitude(const Tuple &tuple) {
    return sqrt(tuple.values[0] * tuple.values[0]
              + tuple.values[1] * tuple.values[1]
              + tuple.values[2] * tuple.values[2]
              + tuple.values[3] * tuple.values[3]);
}

Tuple Tuple::normalize(const Tuple &tuple) {
    double mgn = magnitude(tuple);
    return Tuple(tuple.values[0] / mgn, 
                 tuple.values[1] / mgn, 
                 tuple.values[2] / mgn, 
                 tuple.values[3] / mgn);
}

double Tuple::dot(const Tuple & t1, const Tuple & t2) {
    return t1.values[0] * t2.values[0]
        + t1.values[1] * t2.values[1]
        + t1.values[2] * t2.values[2]
        + t1.values[3] * t2.values[3];
}

Tuple Tuple::cross(const Tuple & t1, const Tuple & t2) {
    return vector(t1.values[1] * t2.values[2] - t1.values[2] * t2.values[1],
                  t1.values[2] * t2.values[0] - t1.values[0] * t2.values[2],
                  t1.values[0] * t2.values[1] - t1.values[1] * t2.values[0]);
}

Tuple Tuple::reflect(const Tuple & in, const Tuple & normal)
{
    return in - normal * 2 * dot(in, normal);
}
