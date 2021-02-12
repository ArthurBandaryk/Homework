#include "triangle_indexed_render.hxx"

TriangleIndexedRender::TriangleIndexedRender(canv::Image& img)
    : TriangleRender(img)
{
}

void TriangleIndexedRender::draw_triangles_indexed(
    canv::pixels& v_vertexes, std::vector<short>& v_indexes, canv::Color& col)
{
    canv::pixels triangles_pixels_for_draw;
    size_t       size_index_buf = v_indexes.size();
    for (size_t i = 0; i < size_index_buf / 3; ++i)
    {
        short index0 = v_indexes[i * 3];
        short index1 = v_indexes[i * 3 + 1];
        short index2 = v_indexes[i * 3 + 2];

        canv::Position v0 = v_vertexes.at(index0);
        canv::Position v1 = v_vertexes.at(index1);
        canv::Position v2 = v_vertexes.at(index2);

        canv::pixels temp = pixels_pos(v0, v1, v2);

        for (const auto& el : temp)
            triangles_pixels_for_draw.emplace_back(el);
    }

    for (auto& el : triangles_pixels_for_draw)
    {
        this->set_pixel(el, col);
    }
}
