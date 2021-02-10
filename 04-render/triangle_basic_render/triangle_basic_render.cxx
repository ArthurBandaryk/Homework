#include "triangle_basic_render.hxx"

TriangleRender::TriangleRender(canv::Image& img)
    : LineRender(img)
{
}

canv::pixels TriangleRender::pixels_pos(canv::Position& v0,
                                        canv::Position& v1,
                                        canv::Position& v2)
{
    canv::pixels res;
    canv::pixels line1{ LineRender::pixels_positions(v0, v1) };
    canv::pixels line2{ LineRender::pixels_positions(v1, v2) };
    canv::pixels line3{ LineRender::pixels_positions(v2, v0) };
    for (const auto& el : line1)
        res.emplace_back(el);
    for (const auto& el : line2)
        res.emplace_back(el);
    for (const auto& el : line3)
        res.emplace_back(el);
    return res;
}

void TriangleRender::draw_triangles(canv::pixels& vertexes,
                                    size_t        num_vertexes,
                                    canv::Color&  col)
{
    canv::pixels triangles_pixels_for_draw;
    for (size_t i = 0; i < num_vertexes / 3; ++i)
    {
        canv::Position v0   = vertexes.at(i * 3);
        canv::Position v1   = vertexes.at(i * 3 + 1);
        canv::Position v2   = vertexes.at(i * 3 + 2);
        canv::pixels   temp = pixels_pos(v0, v1, v2);
        for (const auto& el : temp)
            triangles_pixels_for_draw.emplace_back(el);
    }

    for (auto& el : triangles_pixels_for_draw)
    {
        this->set_pixel(el, col);
    }
}
