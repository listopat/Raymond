#include <Matrix.h>
#include <math.h> 
#include <stdexcept>

static const double EPSILON = 0.00001;
static const double identityValues[16] = { 1.0, 0.0, 0.0, 0.0,
                                        0.0, 1.0, 0.0, 0.0,
                                        0.0, 0.0, 1.0, 0.0,
                                        0.0, 0.0, 0.0, 1.0};

Matrix::Matrix(int s) : size(s) {
    grid = std::unique_ptr<double[]>(new double[s * s]{0.0});
}

Matrix::Matrix(int s, const double * values) : size(s)
{
    grid = std::unique_ptr<double[]>(new double[s * s]{ 0.0});
    for (int i = 0; i < s * s; i++) {
        grid[i] = values[i];
    }
}

Matrix::Matrix(const Matrix &m) : size(m.size)
{
    grid = std::unique_ptr<double[]>(new double[size * size]{ 0.0 });
    for (int i = 0; i < size * size; i++) {
        grid[i] = m.grid[i];
    }
}

Matrix Matrix::getIdentity4x4()
{
    return Matrix(4, identityValues);
}

Matrix Matrix::translation(double x, double y, double z)
{
    Matrix result = Matrix(4, identityValues);
    result(0, 3) = x;
    result(1, 3) = y;
    result(2, 3) = z;
    return result;
}

Matrix Matrix::scaling(double x, double y, double z)
{
    Matrix result = Matrix(4, identityValues);
    result(0, 0) = x;
    result(1, 1) = y;
    result(2, 2) = z;
    return result;
}

Matrix Matrix::rotationX(double angle)
{
    Matrix result = Matrix(4, identityValues);
    result(1, 1) = cos(angle);
    result(2, 2) = cos(angle);
    result(1, 2) = -sin(angle);
    result(2, 1) = sin(angle);
    return result;
}

Matrix Matrix::rotationY(double angle)
{
    Matrix result = Matrix(4, identityValues);
    result(0, 0) = cos(angle);
    result(2, 2) = cos(angle);
    result(2, 0) = -sin(angle);
    result(0, 2) = sin(angle);
    return result;
}

Matrix Matrix::rotationZ(double angle)
{
    Matrix result = Matrix(4, identityValues);
    result(0, 0) = cos(angle);
    result(1, 1) = cos(angle);
    result(0, 1) = -sin(angle);
    result(1, 0) = sin(angle);
    return result;
}

Matrix Matrix::shearing(double x_y, double x_z, double y_x, double y_z, double z_x, double z_y)
{
    Matrix result = Matrix(4, identityValues);
    result(0, 1) = x_y;
    result(0, 2) = x_z;
    result(1, 0) = y_x;
    result(1, 2) = y_z;
    result(2, 0) = z_x;
    result(2, 1) = z_y;
    return result;
}

const Matrix Matrix::identity4x4 = Matrix(4, identityValues);

Matrix & Matrix::operator=(const Matrix & m)
{
    if (size != m.size) {
        throw std::invalid_argument("Sizes don't match");
    }

    for (int i = 0; i < size * size; i++) {
        grid[i] = m.grid[i];
    }
}

double Matrix::operator()(int row, int column) const
{
    return grid[row * size + column];
}

double & Matrix::operator()(int row, int column)
{
    return grid[row * size + column];
}

bool Matrix::operator==(const Matrix & other) const
{
    if (size != other.size) {
        return false;
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (fabs((*this)(i, j) - other(i, j)) >= EPSILON) {
                return false;
            }
        }
    }

    return true;
}

Matrix Matrix::operator*(const Matrix & other) const
{
    Matrix result = Matrix(4);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result(i, j) = (*this)(i, 0) * other(0, j)
                         + (*this)(i, 1) * other(1, j)
                         + (*this)(i, 2) * other(2, j)
                         + (*this)(i, 3) * other(3, j);
        }
    }
    return result;
}

Tuple Matrix::operator*(const Tuple & other) const
{
    Tuple result = Tuple::Tuple(0, 0, 0, 0);
    for (int i = 0; i < 4; i++) {
        result[i] = (*this)(i, 0) * other[0]
                  + (*this)(i, 1) * other[1]
                  + (*this)(i, 2) * other[2]
                  + (*this)(i, 3) * other[3];
    }
    return result;
}

Matrix Matrix::transpose(const Matrix &m)
{
    Matrix result = Matrix(m.size);
    for (int i = 0; i < m.size; i++) {
        for (int j = i; j < m.size; j++) {
            result(i, j) = m(j, i);
            result(j, i) = m(i, j);
        }
    }
    return result;
}

double Matrix::determinant(const Matrix & m)
{
    double result = 0.0f;
    if (m.size == 2) {
        result = m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0);
    }
    else {
        for (int i = 0; i < m.size; i++) {
            result += m(0, i) * cofactor(m, 0, i);
        }
    }
    return result;
}

Matrix Matrix::submatrix(const Matrix & m, int row, int column)
{
    Matrix result = Matrix(m.size - 1);
    int currentRow = 0;
    int currentColumn = 0;

    for (int i = 0; i < m.size; i++) {
        if (i != row) {
            currentColumn = 0;
            for (int j = 0; j < m.size; j++) {
                if (j != column) {
                    result(currentRow, currentColumn) = m(i, j);
                    currentColumn++;
                }
            }
            currentRow++;
        }
    }
    return result;
}

double Matrix::minor(const Matrix & m, int row, int column)
{
    return determinant(submatrix(m, row, column));
}

double Matrix::cofactor(const Matrix & m, int row, int column)
{
    return (row + column) % 2 == 0 ? minor(m, row, column) : -1 * minor(m, row, column);
}

bool Matrix::isInvertible(const Matrix & m)
{
    return determinant(m) != 0;
}

Matrix Matrix::inverse(const Matrix & m)
{
    if (!isInvertible(m)) {
        throw std::invalid_argument("received noninversible matrix");
    }

    double d = determinant(m);
    Matrix result = Matrix(m.size);
    for (int i = 0; i < m.size; i++) {
        for (int j = 0; j < m.size; j++) {
            double c = cofactor(m, i, j);
            result(j, i) = c / d;
        }
    }
    return result;
}

Matrix Matrix::viewTransform(const Tuple &from, const Tuple &to, const Tuple &up)
{
    Tuple forward = Tuple::normalize(to - from);
    Tuple upn = Tuple::normalize(up);
    Tuple left = Tuple::cross(forward, upn);
    Tuple trueUp = Tuple::cross(left, forward);

    double values[16] = {left[0], left[1], left[2], 0,
                         trueUp[0], trueUp[1], trueUp[2], 0,
                         -forward[0], -forward[1], -forward[2], 0,
                         0, 0, 0, 1 };
    Matrix orientation = Matrix(4, values);
    return orientation * Matrix::translation(-from[0], -from[1], -from[2]);
}
