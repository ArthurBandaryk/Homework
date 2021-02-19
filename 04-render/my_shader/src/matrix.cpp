#include "matrix.hxx"
#include <cmath>

Matrix::Matrix() {}

Matrix Matrix::multiply(Matrix& m1, Matrix& m2)
{
    Matrix m;
    for (size_t i = 0; i < 4; ++i)
    {
        for (size_t j = 0; j < 4; ++j)
        {
            // clang-format off
            m.matrix[i][j] =
                      m1.matrix[i][0] * m2.matrix[0][j] +
                      m1.matrix[i][1] * m2.matrix[1][j] +
                      m1.matrix[i][2] * m2.matrix[2][j] +
                      m1.matrix[i][3] * m2.matrix[3][j];
            // clang-format on
        }
    }
    return m;
}

Vector Matrix::multiply_vect(const Vector& v)
{
    // clang-format off
    return Vector{
        matrix[0][0] * v.x + matrix[0][1] * v.y + matrix[0][2] * v.z + matrix[0][3] * v.w,
        matrix[1][0] * v.x + matrix[1][1] * v.y + matrix[1][2] * v.z + matrix[1][3] * v.w,
        matrix[2][0] * v.x + matrix[2][1] * v.y + matrix[2][2] * v.z + matrix[2][3] * v.w,
        matrix[3][0] * v.x + matrix[3][1] * v.y + matrix[3][2] * v.z + matrix[3][3] * v.w,
      };
    // clang-format on
}

void Matrix::set_scale_matrix(float sx, float sy, float sz) // scale
{
    matrix[0][0] = sx;
    matrix[0][1] = 0.f;
    matrix[0][2] = 0.f;
    matrix[0][3] = 0.f;

    matrix[1][0] = 0.f;
    matrix[1][1] = sy;
    matrix[1][2] = 0.f;
    matrix[1][3] = 0.f;

    matrix[2][0] = 0.f;
    matrix[2][1] = 0.f;
    matrix[2][2] = sz;
    matrix[2][3] = 0.f;

    matrix[3][0] = 0.f;
    matrix[3][1] = 0.f;
    matrix[3][2] = 0.f;
    matrix[3][3] = 1.f;
}

void Matrix::set_rotateX_matrix(float angle) // rotate through x-axis
{
    constexpr float c_pi = 3.14159;
    float           alfa = angle * c_pi / 180.f;

    matrix[0][0] = 1.0;
    matrix[0][1] = 0.f;
    matrix[0][2] = 0.f;
    matrix[0][3] = 0.f;

    matrix[1][0] = 0.f;
    matrix[1][1] = static_cast<float>(cos(alfa));
    matrix[1][2] = static_cast<float>(-sin(alfa));
    matrix[1][3] = 0.f;

    matrix[2][0] = 0.f;
    matrix[2][1] = static_cast<float>(sin(alfa));
    matrix[2][2] = static_cast<float>(cos(alfa));
    matrix[2][3] = 0.f;

    matrix[3][0] = 0.f;
    matrix[3][1] = 0.f;
    matrix[3][2] = 0.f;
    matrix[3][3] = 1.f;
}

void Matrix::set_rotateY_matrix(float angle) // rotate through y-axis
{
    constexpr float c_pi = 3.14159;
    float           alfa = angle * c_pi / 180.f;

    matrix[0][0] = static_cast<float>(cos(alfa));
    matrix[0][1] = 0.f;
    matrix[0][2] = static_cast<float>(sin(alfa));
    matrix[0][3] = 0.f;

    matrix[1][0] = 0.f;
    matrix[1][1] = 1.f;
    matrix[1][2] = 0.f;
    matrix[1][3] = 0.f;

    matrix[2][0] = static_cast<float>(-sin(alfa));
    matrix[2][1] = 0.f;
    matrix[2][2] = static_cast<float>(cos(alfa));
    matrix[2][3] = 0.f;

    matrix[3][0] = 0.f;
    matrix[3][1] = 0.f;
    matrix[3][2] = 0.f;
    matrix[3][3] = 1.f;
}

void Matrix::set_rotateZ_matrix(float angle) // rotate through z-axis
{
    constexpr float c_pi = 3.14159;
    float           alfa = angle * c_pi / 180.f;

    matrix[0][0] = static_cast<float>(cos(alfa));
    matrix[0][1] = static_cast<float>(-sin(alfa));
    matrix[0][2] = 0.f;
    matrix[0][3] = 0.f;

    matrix[1][0] = static_cast<float>(sin(alfa));
    matrix[1][1] = static_cast<float>(cos(alfa));
    matrix[1][2] = 0.f;
    matrix[1][3] = 0.f;

    matrix[2][0] = 0.f;
    matrix[2][1] = 0.f;
    matrix[2][2] = 1.f;
    matrix[2][3] = 0.f;

    matrix[3][0] = 0.f;
    matrix[3][1] = 0.f;
    matrix[3][2] = 0.f;
    matrix[3][3] = 1.f;
}
