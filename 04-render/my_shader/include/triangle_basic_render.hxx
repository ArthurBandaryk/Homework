#pragma once

#include "line_render.hxx"

class TriangleRender : public LineRender
{
public:
    TriangleRender(canv::Image& img);
    canv::pixels pixels_pos(canv::Position& v0,
                            canv::Position& v1,
                            canv::Position& v2);
    // void set_color_line(canv::pixels& pixs);
    void draw_triangles(canv::pixels& vertexes,
                        size_t        num_vertexes,
                        canv::Color&  col);
};
