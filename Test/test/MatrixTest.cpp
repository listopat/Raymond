#include <catch.hpp>
#include <Matrix.h>
#include <Tuple.h>

#include <iostream>
#include <math.h>  

static const double EPSILON = 0.00001;
static const double PI = 3.14159265;

TEST_CASE("Matrix construction and operators working properly", "[matrix]") {
    SECTION("Constructing and inspecting a 4x4 matrix") {
        double values[16] = {1.0, 2.0, 3.0, 4.0,
                            5.5, 6.5, 7.5, 8.5,
                            9.0, 10.0, 11.0, 12.0,
                            13.5, 14.5, 15.5, 16.5};
        Matrix m = Matrix::Matrix(4, values);
        REQUIRE(m(0, 0) == 1);
        REQUIRE(m(0, 3) == 4);
        REQUIRE(m(1, 0) == 5.5);
        REQUIRE(m(1, 2) == 7.5);
        REQUIRE(m(2, 2) == 11);
        REQUIRE(m(3, 0) == 13.5);
        REQUIRE(m(3, 2) == 15.5);
    }

    SECTION("A 2x2 matrix ought to be representable") {
        double values[4] = {-3.0, 5.0,
                            1.0, -2.0};
        Matrix m = Matrix::Matrix(2, values);
        REQUIRE(m(0, 0) == -3.0);
        REQUIRE(m(0, 1) == 5.0);
        REQUIRE(m(1, 0) == 1.0);
        REQUIRE(m(1, 1) == -2.0);
    }

    SECTION("A 3x3 matrix ought to be representable") {
        double values[9] = { -3.0, 5.0, 1.0,
                            -3.0, -2.0, 1.0,
                            -3.0, 5.0, 1.0};
        Matrix m = Matrix::Matrix(3, values);
        REQUIRE(m(0, 0) == -3.0);
        REQUIRE(m(1, 1) == -2.0);
        REQUIRE(m(2, 2) == 1.0);
    }

    SECTION("Matrix equality with identical matrices") {
        double values[16] = {1.0, 2.0, 3.0, 4.0,
                            5.5, 6.5, 7.5, 8.5,
                            9.0, 10.0, 11.0, 12.0,
                            13.5, 14.5, 15.5, 16.5 };
        Matrix a = Matrix::Matrix(4, values);
        Matrix b = Matrix::Matrix(4, values);
        REQUIRE((a == b));
    }

    SECTION("Matrix equality with different matrices") {
        double values[16] = { 1.0, 2.0, 3.0, 4.0,
                            5.5, 6.5, 7.5, 8.5,
                            9.0, 10.0, 11.0, 12.0,
                            13.5, 14.5, 15.5, 16.5 };
        Matrix a = Matrix::Matrix(4, values);
        values[0] = 0.0;
        Matrix b = Matrix::Matrix(4, values);
        REQUIRE(!(a == b));
    }
}

TEST_CASE("Matrix operations working properly", "[matrix]") {
    SECTION("Multiplying matrices") {
        double values1[16] = { 1.0, 2.0, 3.0, 4.0 ,
                            5.0, 6.0, 7.0, 8.0,
                            9.0, 8.0, 7.0, 6.0,
                            5.0, 4.0, 3.0, 2.0};
        double values2[16] = { -2.0, 1.0, 2.0, 3.0,
                            3.0, 2.0, 1.0, -1.0,
                            4.0, 3.0, 6.0, 5.0,
                            1.0, 2.0, 7.0, 8.0};
        double values3[16] = { 20.0, 22.0, 50.0, 48.0,
                            44.0, 54.0, 114.0, 108.0,
                            40.0, 58.0, 110.0, 102.0,
                            16.0, 26.0, 46.0, 42.0};
        Matrix a = Matrix::Matrix(4, values1);
        Matrix b = Matrix::Matrix(4, values2);
        Matrix c = Matrix::Matrix(4, values3);
        REQUIRE((a * b == c));
    }
    SECTION("A matrix multiplied by a tuple") {
        double values[16] = { 1.0, 2.0, 3.0, 4.0 ,
                            2.0, 4.0, 4.0, 2.0,
                            8.0, 6.0, 4.0, 1.0,
                            0.0, 0.0, 0.0, 1.0 };
        Matrix A = Matrix::Matrix(4, values);
        Tuple b = Tuple::Tuple(1, 2, 3, 1);
        REQUIRE((A * b == Tuple::Tuple(18, 24, 33, 1)));
    }

    SECTION("Multiplying a matrix by the identity matrix") {
        double values[16] = { 1.0, 2.0, 3.0, 4.0 ,
                    2.0, 4.0, 4.0, 2.0,
                    8.0, 6.0, 4.0, 1.0,
                    0.0, 0.0, 0.0, 1.0 };
        Matrix A = Matrix::Matrix(4, values);
        REQUIRE((Matrix::identity4x4 * A == A));
    }

    SECTION("Multiplying the identity matrix by a tuple") {
        Tuple a = Tuple::Tuple(1, 2, 3, 1);
        REQUIRE((Matrix::identity4x4 * a == a));
    }

    SECTION("Transposing a matrix") {
        double values1[16] = { 0, 9, 3, 0,
                              9, 8, 0, 8,
                              1, 8, 5, 3,
                              0, 0, 5, 8};
        double values2[16] = { 0, 9, 1, 0,
                              9, 8, 8, 0,
                              3, 0, 5, 5,
                              0, 8, 3, 8 };
        Matrix A = Matrix::Matrix(4, values1);
        Matrix B = Matrix::Matrix(4, values2);
        REQUIRE((Matrix::transpose(A) == B));
    }

    SECTION("Calculating the determinant of a 2x2 matrix") {
        double values[4] = { 1, 5, -3, 2 };
        Matrix A = Matrix::Matrix(2, values);
        REQUIRE((Matrix::determinant(A) == 17));
    }

    SECTION("A submatrix of a 3x3 matrix is a 2x2 matrix") {
        double values1[9] = { 1, 5, 0,
                            -3, 2, 7,
                            0, 6, -3};
        double values2[4] = { -3, 2, 0, 6};
        Matrix A = Matrix::Matrix(3, values1);
        Matrix B = Matrix::Matrix(2, values2);
                
        REQUIRE((Matrix::submatrix(A, 0, 2) == B));
    }

    SECTION("A submatrix of a 4x4 matrix is a 3x3 matrix") {
        double values1[16] = { -6, 1, 1, 6,
                              -8, 5, 8, 6,
                              -1, 0, 8, 2,
                              -7, 1, -1, 1};
        double values2[9] = {-6, 1, 6,
                            -8, 8, 6,
                            -7, -1, 1};
        Matrix A = Matrix::Matrix(4, values1);
        Matrix B = Matrix::Matrix(3, values2);
        REQUIRE((Matrix::submatrix(A, 2, 1) == B));
    }

    SECTION("Calculating a minor of a 3x3 matrix") {
        double values[9] = { 3, 5, 0,
                            2, -1, -7,
                            6, -1, 5};
        Matrix A = Matrix::Matrix(3, values);
        Matrix B = Matrix::submatrix(A, 1, 0);
        REQUIRE((Matrix::determinant(B) == 25));
        REQUIRE((Matrix::minor(A, 1, 0) == 25));
    }

    SECTION("Calculating a cofactor of a 3x3 matrix") {
        double values[9] = { 3, 5, 0,
                            2, -1, -7,
                            6, -1, 5 };
        Matrix A = Matrix::Matrix(3, values);
        REQUIRE((Matrix::minor(A, 0, 0) == -12));
        REQUIRE((Matrix::cofactor(A, 0, 0) == -12));
        REQUIRE((Matrix::minor(A, 1, 0) == 25));
        REQUIRE((Matrix::cofactor(A, 1, 0) == -25));
    }

    SECTION("Calculating the determinant of a 3x3 matrix") {
        double values[9] = { 1, 2, 6,
                            -5, 8, -4,
                            2, 6, 4};
        Matrix A = Matrix::Matrix(3, values);
        REQUIRE((Matrix::cofactor(A, 0, 0) == 56));
        REQUIRE((Matrix::cofactor(A, 0, 1) == 12));
        REQUIRE((Matrix::cofactor(A, 0, 2) == -46));
        REQUIRE((Matrix::determinant(A) == -196));
    }


    SECTION("Calculating the determinant of a 4x4 matrix") {
        double values[16] = { -2, -8, 3, 5,
                              -3, 1, 7, 3,
                              1, 2, -9, 6,
                              -6, 7, 7, -9};
        Matrix A = Matrix::Matrix(4, values);
        REQUIRE((Matrix::cofactor(A, 0, 0) == 690));
        REQUIRE((Matrix::cofactor(A, 0, 1) == 447));
        REQUIRE((Matrix::cofactor(A, 0, 2) == 210));
        REQUIRE((Matrix::cofactor(A, 0, 3) == 51));
        REQUIRE((Matrix::determinant(A) == -4071));
    }

    SECTION("Testing an invertible matrix invertibility") {
        double values[16] = { 6, 4, 4, 4,
                             5, 5, 7, 6,
                             4, -9, 3, -7,
                             9, 1, 7, -6};
        Matrix A = Matrix::Matrix(4, values);
        REQUIRE((Matrix::determinant(A) == -2120));
        REQUIRE(Matrix::isInvertible(A));
    }

    SECTION("Testing an noninvertible matrix invertibility") {
        double values[16] = { -4, 2, -2, -3,
                             9, 6, 2, 6,
                             0, -5, 1, -5,
                             0, 0, 0, 0};
        Matrix A = Matrix::Matrix(4, values);
        REQUIRE((Matrix::determinant(A) == 0));
        REQUIRE(!Matrix::isInvertible(A));
    }

    SECTION("Calculating the inverse of a matrix") {
        double values1[16] = {-5, 2, 6, -8,
                             1, -5, 1, 8,
                             7, 7, -6, -7,
                             1, -3, 7, 4};
        double values2[16] = {0.21805, 0.45113, 0.24060, -0.04511,
                             -0.80827, -1.45677, -0.44361, 0.52068,
                             -0.07895, -0.22368, -0.05263, 0.19737,
                             -0.52256, -0.81391, -0.30075, 0.30639};
        Matrix A = Matrix::Matrix(4, values1);
        Matrix B = Matrix::Matrix(4, values2);
        REQUIRE((Matrix::determinant(A) == 532));
        REQUIRE((Matrix::cofactor(A, 2, 3) ==  -160));
        REQUIRE(abs(B(3, 2) - (-160.0 / 532.0)) < EPSILON);
        REQUIRE((Matrix::cofactor(A, 3, 2) == 105));
        REQUIRE(abs(B(2, 3) - (105.0 / 532.0)) < EPSILON);
        REQUIRE((Matrix::inverse(A) == B));
    }

    SECTION("Calculating the inverse of another matrix") {
        double values1[16] = {8, -5, 9, 2,
                             7, 5, 6, 1,
                             -6, 0, 9, 6,
                             -3, 0, -9, -4};
        double values2[16] = { -0.15385, -0.15385, -0.28205, -0.53846,
                             -0.07692, 0.12308, 0.02564, 0.03077,
                             0.35897, 0.35897, 0.43590, 0.92308,
                             -0.69231, -0.69231, -0.76923, -1.92308};
        Matrix A = Matrix::Matrix(4, values1);
        Matrix B = Matrix::Matrix(4, values2);
        REQUIRE((Matrix::inverse(A) == B));
    }

    SECTION("Calculating the inverse of a third matrix") {
        double values1[16] = {9, 3, 0, 9,
                             -5, -2, -6, -3,
                             -4, 9, 6, 4,
                             -7, 6, 6, 2};
        double values2[16] = { -0.04074, -0.07778, 0.14444, -0.22222,
                             -0.07778, 0.03333, 0.36667, -0.33333,
                             -0.02901, -0.14630, -0.10926, 0.12963,
                             0.17778, 0.06667, -0.26667, 0.33333};
        Matrix A = Matrix::Matrix(4, values1);
        Matrix B = Matrix::Matrix(4, values2);
        REQUIRE((Matrix::inverse(A) == B));
    }

    SECTION("Multiplying a product by its inverse") {
        double values1[16] = {3, -9, 7, 3,
                             3, -8, 2, -9,
                             -4, 4, 4, 1,
                             -6, 5, -1, 1};
        double values2[16] = {8, 2, 2, 2,
                             3, -1, 7, 0,
                             7, 0, 5, 4,
                             6, -2, 0, 5};
        Matrix A = Matrix::Matrix(4, values1);
        Matrix B = Matrix::Matrix(4, values2);
        Matrix C = A * B;
        REQUIRE((C * Matrix::inverse(B) == A));
    }
}

TEST_CASE("Matrix transformations working properly", "[matrixtransformations]") {
    SECTION("Multiplying by a translation matrix") {
        Matrix transform = Matrix::translation(5, -3, 2);
        Tuple p = Tuple::point(-3, 4, 5);
        REQUIRE((transform * p == Tuple::point(2, 1, 7)));
    }

    SECTION("Multiplying by the inverse of a translation matrix") {
        Matrix transform = Matrix::translation(5, -3, 2);
        Matrix inverse = Matrix::inverse(transform);
        Tuple p = Tuple::point(-3, 4, 5);
        REQUIRE((inverse * p == Tuple::point(-8, 7, 3)));
    }

    SECTION("Translation does not affect vectors") {
        Matrix transform = Matrix::translation(5, -3, 2);
        Tuple v = Tuple::vector(-3, 4, 5);
        REQUIRE((transform * v == v));
    }

    SECTION("A scaling matrix applied to a point") {
        Matrix transform = Matrix::scaling(2, 3, 4);
        Tuple p = Tuple::point(-4, 6, 8);
        REQUIRE((transform * p == Tuple::point(-8, 18, 32)));
    }

    SECTION("A scaling matrix applied to a vector") {
        Matrix transform = Matrix::scaling(2, 3, 4);
        Tuple v = Tuple::vector(-4, 6, 8);
        REQUIRE((transform * v == Tuple::vector(-8, 18, 32)));
    }

    SECTION("Multiplying by the inverse of a scaling matrix") {
        Matrix transform = Matrix::scaling(2, 3, 4);
        Matrix inv = Matrix::inverse(transform);
        Tuple v = Tuple::vector(-4, 6, 8);
        REQUIRE((inv * v == Tuple::vector(-2, 2, 2)));
    }

    SECTION("Reflection is scaling by a negative value") {
        Matrix transform = Matrix::scaling(-1, 1, 1);
        Tuple p = Tuple::point(2, 3, 4);
        REQUIRE((transform * p == Tuple::point(-2, 3, 4)));
    }

    SECTION("Rotating a point around the x axis") {
        Tuple p = Tuple::point(0, 1, 0);
        Matrix half_quarter = Matrix::rotationX(PI / 4);
        Matrix full_quarter = Matrix::rotationX(PI / 2);
        REQUIRE((half_quarter * p == Tuple::point(0, sqrt(2.0)/2.0, sqrt(2.0)/2.0)));
        REQUIRE((full_quarter * p == Tuple::point(0, 0, 1)));
    }

    SECTION("The inverse of an x-rotation rotates in the opposite direction") {
        Tuple p = Tuple::point(0, 1, 0);
        Matrix half_quarter = Matrix::rotationX(PI / 4);
        Matrix inv = Matrix::inverse(half_quarter);
        REQUIRE((inv * p == Tuple::point(0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0)));
    }

    SECTION("Rotating a point around the y axis") {
        Tuple p = Tuple::point(0, 0, 1);
        Matrix half_quarter = Matrix::rotationY(PI / 4);
        Matrix full_quarter = Matrix::rotationY(PI / 2);
        REQUIRE((half_quarter * p == Tuple::point(sqrt(2.0) / 2.0, 0, sqrt(2.0) / 2.0)));
        REQUIRE((full_quarter * p == Tuple::point(1, 0, 0)));
    }

    SECTION("Rotating a point around the z axis") {
        Tuple p = Tuple::point(0, 1, 0);
        Matrix half_quarter = Matrix::rotationZ(PI / 4);
        Matrix full_quarter = Matrix::rotationZ(PI / 2);
        REQUIRE((half_quarter * p == Tuple::point(-sqrt(2.0) / 2.0, sqrt(2.0) / 2.0, 0)));
        REQUIRE((full_quarter * p == Tuple::point(-1, 0, 0)));
    }

    SECTION("A shearing transformation moves x in proportion to y") {
        Matrix transform = Matrix::shearing(1, 0, 0, 0, 0, 0);
        Tuple p = Tuple::point(2, 3, 4);
        REQUIRE((transform * p == Tuple::point(5, 3, 4)));
    }

    SECTION("A shearing transformation moves x in proportion to z") {
        Matrix transform = Matrix::shearing(0, 1, 0, 0, 0, 0);
        Tuple p = Tuple::point(2, 3, 4);
        REQUIRE((transform * p == Tuple::point(6, 3, 4)));
    }

    SECTION("A shearing transformation moves y in proportion to x") {
        Matrix transform = Matrix::shearing(0, 0, 1, 0, 0, 0);
        Tuple p = Tuple::point(2, 3, 4);
        REQUIRE((transform * p == Tuple::point(2, 5, 4)));
    }

    SECTION("A shearing transformation moves y in proportion to z") {
        Matrix transform = Matrix::shearing(0, 0, 0, 1, 0, 0);
        Tuple p = Tuple::point(2, 3, 4);
        REQUIRE((transform * p == Tuple::point(2, 7, 4)));
    }

    SECTION("A shearing transformation moves z in proportion to x") {
        Matrix transform = Matrix::shearing(0, 0, 0, 0, 1, 0);
        Tuple p = Tuple::point(2, 3, 4);
        REQUIRE((transform * p == Tuple::point(2, 3, 6)));
    }

    SECTION("A shearing transformation moves z in proportion to y") {
        Matrix transform = Matrix::shearing(0, 0, 0, 0, 0, 1);
        Tuple p = Tuple::point(2, 3, 4);
        REQUIRE((transform * p == Tuple::point(2, 3, 7)));
    }

    SECTION("Individual transformations are applied in sequence") {
        Tuple p = Tuple::point(1, 0, 1);
        Matrix A = Matrix::rotationX(PI / 2);
        Matrix B = Matrix::scaling(5.0, 5.0, 5.0);
        Matrix C = Matrix::translation(10, 5, 7);
        Matrix D = Matrix::rotationX(PI / 2);

        Tuple p2 = A * p;
        REQUIRE((p2 == Tuple::point(1, -1, 0)));
        Tuple p3 = B * p2;
        REQUIRE((p3 == Tuple::point(5, -5, 0)));
        Tuple p4 = C * p3;
        REQUIRE((p4 == Tuple::point(15, 0, 7)));
    }

    SECTION("Chained transformations must be applied in reverse order") {
        Tuple p = Tuple::point(1, 0, 1);
        Matrix A = Matrix::rotationX(PI / 2);
        Matrix B = Matrix::scaling(5.0, 5.0, 5.0);
        Matrix C = Matrix::translation(10.0, 5.0, 7.0);
        Matrix transform = C * B * A;
        REQUIRE((transform * p == Tuple::point(15, 0, 7)));
    }

    SECTION("The transformation matrix for the default orientation") {
        Tuple from = Tuple::point(0, 0, 0);
        Tuple to = Tuple::point(0, 0, -1);
        Tuple up = Tuple::vector(0, 1, 0);
        Matrix t = Matrix::viewTransform(from, to, up);
        REQUIRE(t == Matrix::identity4x4);
    }

    SECTION("A view transformation matrix looking in positive z direction") {
        Tuple from = Tuple::point(0, 0, 0);
        Tuple to = Tuple::point(0, 0, 1);
        Tuple up = Tuple::vector(0, 1, 0);
        Matrix t = Matrix::viewTransform(from, to, up);
        REQUIRE(t == Matrix::scaling(-1, 1, -1));
    }

    SECTION("The view transformation moves the world") {
        Tuple from = Tuple::point(0, 0, 8);
        Tuple to = Tuple::point(0, 0, 0);
        Tuple up = Tuple::vector(0, 1, 0);
        Matrix t = Matrix::viewTransform(from, to, up);
        REQUIRE(t == Matrix::translation(0, 0, -8));
    }

    SECTION("An arbitrary view transformation") {
        Tuple from = Tuple::point(1, 3, 2);
        Tuple to = Tuple::point(4, -2, 8);
        Tuple up = Tuple::vector(1, 1, 0);
        Matrix t = Matrix::viewTransform(from, to, up);
        double values[16] = {-0.50709, 0.50709, 0.67612, -2.36643,
                              0.76772, 0.60609, 0.12122, -2.82843,
                              -0.35857, 0.59761, -0.71714, 0.00000,
                              0.00000, 0.00000, 0.00000, 1.00000};
        Matrix m = Matrix::Matrix(4, values);
        REQUIRE(t == m);
    }
}