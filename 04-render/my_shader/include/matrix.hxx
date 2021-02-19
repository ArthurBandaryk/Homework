#ifndef MATRIX_HXX
#define MATRIX_HXX

#include "vector.hxx"
#include <cstddef>

class Matrix
{
private:
    // clang-format off
    float matrix[4][4] = {
        {0.f, 0.f, 0.f, 0.f},
        {0.f, 0.f, 0.f, 0.f},
        {0.f, 0.f, 0.f, 0.f},
        {0.f, 0.f, 0.f, 0.f}
    };
    // clang-format on

public:
    Matrix();
    static Matrix multiply(Matrix& m1, Matrix& m2);
    Vector        multiply_vect(const Vector& v);
    void          set_scale_matrix(float sx, float sy, float sz);
    void set_rotateX_matrix(float angle); // angle - in degrees
    void set_rotateY_matrix(float angle); // angle - in degrees
    void set_rotateZ_matrix(float angle); // angle - in degrees
};

#endif // MATRIX_HXX
