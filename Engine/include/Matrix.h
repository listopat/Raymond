#pragma once
#include <memory>
#include <Tuple.h>

class Matrix {
public:
    const int size;

    Matrix(int s);
    Matrix(int s, const double* values);
    Matrix(const Matrix &m);
    static Matrix getIdentity4x4();
    static const Matrix identity4x4;
    static Matrix translation(double x, double y, double z);
    static Matrix scaling(double x, double y, double z);
    static Matrix rotationX(double angle);
    static Matrix rotationY(double angle);
    static Matrix rotationZ(double angle);
    static Matrix shearing(double x_y, double x_z, double y_x, double y_z, double z_x, double z_y);

    Matrix& operator=(const Matrix &m);
    double operator()(int row, int column) const;
    double& operator()(int row, int column);
    bool operator==(const Matrix &other) const;
    Matrix operator*(const Matrix &other) const;
    Tuple operator*(const Tuple &other) const;

    static Matrix transpose(const Matrix &m);
    static double determinant(const Matrix &m);
    static Matrix submatrix(const Matrix &m, int row, int column);
    static double minor(const Matrix&m, int row, int column);
    static double cofactor(const Matrix&m, int row, int column);
    static bool isInvertible(const Matrix&m);
    static Matrix inverse(const Matrix&m);
    static Matrix viewTransform(const Tuple &from, const Tuple &to, const Tuple &up);

private:
    std::unique_ptr<double[]> grid;
};