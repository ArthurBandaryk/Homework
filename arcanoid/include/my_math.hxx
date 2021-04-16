#pragma once

#include <array>

struct Vector
{
    float x;
    float y;
    Vector();
    Vector(float x, float y);
};

std::ostream& operator<<(std::ostream& os, const Vector& v);

struct Vertex1
{
    Vector v_pos;
    Vertex1();
    Vertex1(const Vector v);
};

struct Vertex2
{
    Vector v_pos;
    Vector v_texture;
    Vertex2();
    // clang-format off
    Vertex2(Vector v_pos, Vector v_texture);
    // clang-format on
};

struct Triangle1
{
    std::array<Vertex1, 3> triangle;
    Triangle1();
    // clang-format off
    Triangle1(const Vertex1 &v1,
           const Vertex1 &v2,
           const Vertex1 &v3);

    // clang-format on
};

struct Triangle2
{
    std::array<Vertex2, 3> triangle;
    Triangle2();
    // clang-format off
    Triangle2(const Vertex2 &v1,
           const Vertex2 &v2,
           const Vertex2 &v3);

    // clang-format on
};

struct Matrix
{
    Vector        col1;
    Vector        col2;
    Vector        col3;
    static Matrix rotate(float alfa); // alfa in degrees
    static Matrix move(const Vector& v);
    static Matrix scale(float scale);
};

std::ostream& operator<<(std::ostream& os, const Matrix& m);

Vector  operator*(const Vector& v, const Matrix& m);
Vertex2 operator*(const Vertex2& v, const Matrix& m);
Matrix  operator*(const Matrix& m1, const Matrix& m2);
