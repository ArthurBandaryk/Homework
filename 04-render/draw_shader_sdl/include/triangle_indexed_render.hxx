#pragma once

#include "triangle_basic_render.hxx"

class TriangleIndexedRender : public TriangleRender
{
public:
    TriangleIndexedRender(canv::Image& img);
    void draw_triangles_indexed(canv::pixels&       v_vertexes,
                                std::vector<short>& v_indexes,
                                canv::Color&        col);
};
