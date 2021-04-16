#include "my_math.hxx"
#include <cmath>
#include <iostream>

constexpr float pi = 3.14159;

Vector::Vector() {}

Vector::Vector(float x, float y)
{
    this->x = x;
    this->y = y;
}

std::ostream& operator<<(std::ostream& os, const Vector& v)
{
    os << v.x << " " << v.y << std::endl;
    return os;
}

Vertex1::Vertex1()
{
    v_pos = { 0.f, 0.f };
}

// clang-format off
Vertex1::Vertex1(const Vector v)
{
   v_pos = v;
}
// clang-format on

Vertex2::Vertex2()
{
    v_pos     = { 0.f, 0.f };
    v_texture = { 0.f, 0.f };
}

Vertex2::Vertex2(Vector v_pos, Vector v_texture)
{
    this->v_pos     = v_pos;
    this->v_texture = v_texture;
}

Triangle1::Triangle1()
{
    triangle[0] = Vertex1{};
    triangle[1] = Vertex1{};
    triangle[2] = Vertex1{};
}

// clang-format off
Triangle1::Triangle1(const Vertex1& v1,
                   const Vertex1& v2,
                   const Vertex1& v3)
{
    triangle[0] = v1;
    triangle[1] = v2;
    triangle[2] = v3;
}
// clang-format on

Triangle2::Triangle2()
{
    triangle[0] = Vertex2{};
    triangle[1] = Vertex2{};
    triangle[2] = Vertex2{};
}

Triangle2::Triangle2(const Vertex2& v1,
                     const Vertex2& v2,
                     const Vertex2& v3)
{
    triangle[0] = v1;
    triangle[1] = v2;
    triangle[2] = v3;
}

Matrix Matrix::scale(float scale)
{
    Matrix m;
    m.col1.x = scale;
    m.col1.y = 0.f;
    m.col2.x = 0.f;
    m.col2.y = scale;
    m.col3.x = 0.f;
    m.col3.y = 0.f;
    return m;
}

Matrix Matrix::move(const Vector& v)
{
    Matrix m;
    m.col1.x = 1.f;
    m.col1.y = 0.f;
    m.col2.x = 0.f;
    m.col2.y = 1.f;
    m.col3   = v;
    return m;
}

Matrix Matrix::rotate(float alfa) // alfa in degrees
{
    alfa *= pi / 180.f; // in radianes
    Matrix m;
    m.col1.x = std::cos(alfa);
    m.col1.y = std::sin(alfa);
    m.col2.x = -std::sin(alfa);
    m.col2.y = std::cos(alfa);
    m.col3.x = 0.f;
    m.col3.y = 0.f;
    return m;
}

Vector operator*(const Vector& v, const Matrix& m)
{
    Vector result;
    result.x = v.x * m.col1.x + v.y * m.col2.x + m.col3.x;
    result.y = v.x * m.col1.y + v.y * m.col2.y + m.col3.y;
    return result;
}

Vertex2 operator*(const Vertex2& v, const Matrix& m)
{
    Vertex2 result;
    result.v_texture = v.v_texture;
    result.v_pos.x =
        v.v_pos.x * m.col1.x + v.v_pos.y * m.col2.x + m.col3.x;
    result.v_pos.y =
        v.v_pos.x * m.col1.y + v.v_pos.y * m.col2.y + m.col3.y;
    return result;
}

Matrix operator*(const Matrix& m1, const Matrix& m2)
{
    Matrix m;

    m.col1.x = m1.col1.x * m2.col1.x + m1.col2.x * m2.col1.y;
    m.col1.y = m1.col1.y * m2.col1.x + m1.col2.y * m2.col1.y;
    m.col2.x = m1.col1.x * m2.col2.x + m1.col2.x * m2.col2.y;
    m.col2.y = m1.col1.y * m2.col2.x + m1.col2.y * m2.col2.y;
    m.col3.x = m1.col1.x * m2.col3.x + m1.col2.x * m2.col3.y +
               m1.col3.x;
    m.col3.y = m1.col1.y * m2.col3.x + m1.col2.y * m2.col3.y +
               m1.col3.y;
    return m;
}

std::ostream& operator<<(std::ostream& os, const Matrix& m)
{
    os << m.col1.x << " " << m.col2.x << " " << m.col3.x
       << std::endl;
    os << m.col1.y << " " << m.col2.y << " " << m.col3.y
       << std::endl;
    return os;
}
