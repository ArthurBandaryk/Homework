#include "matrix.hxx"
#include <array>
#include <cmath>
#include <iostream>

int main(int /*argc*/, char* /*argv*/[])
{
    // clang-format off
     std::array<Vector, 8> vertexes
    {
        Vector{-1.f, 1.f, 1.f}, // 0 вершина
        Vector{-1.f, 1.f, -1.f}, // 1 вершина
        Vector{1.f, 1.f, -1.f}, // 2 вершина
        Vector{1.f, 1.f, 1.f}, // 3 вершина
        Vector{-1.f, -1.f, 1.f}, // 4 вершина
        Vector{-1.f, -1.f, -1.f}, // 5 вершина
        Vector{1.f, -1.f, -1.f}, // 6 вершина
        Vector{1.f, -1.f, 1.f}, // 7 вершина
    };

    // clang-format on
    Vector tmp;
    Matrix m;
    for (auto& el : vertexes)
    {
        m.set_scale_matrix(10.f, 10.f, 10.f);
        tmp = m.multiply_vect(el);
        m.set_rotateX_matrix(10.f);
        tmp = m.multiply_vect(tmp);
        m.set_rotateY_matrix(20.f);
        tmp = m.multiply_vect(tmp);
        std::cout << "x = " << tmp.x << " y = " << tmp.y << std::endl;
    }
    return EXIT_SUCCESS;
}
