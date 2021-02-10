#include "triangle_indexed_render.hxx"

int main(int /*argc*/, char* /*argv*/[])
{
    canv::Color black = { 0, 0, 0 };
    canv::Color green = { 0, 255, 0 };

    canv::Image img;

    canv::pixels v_vertexes;

    constexpr size_t num_cells_width  = 10;
    constexpr size_t num_cells_height = 10;

    constexpr size_t width_cell  = (canv::width - 1) / num_cells_width;
    constexpr size_t height_cell = (canv::height - 1) / num_cells_height;

    for (size_t i = 0; i <= num_cells_width; ++i)
    {
        for (size_t j = 0; j <= num_cells_height; ++j)
        {
            canv::Position v{ static_cast<int32_t>(j * width_cell),
                              static_cast<int32_t>(i * height_cell) };

            v_vertexes.emplace_back(v);
        }
    }

    std::vector<short> v_indexes;

    for (size_t i = 0; i < num_cells_width; ++i)
    {
        for (size_t j = 0; j < num_cells_height; ++j)
        {
            short v0 = static_cast<short>(i * (num_cells_width + 1) + j);
            short v1 = static_cast<short>(i * (num_cells_width + 1) +
                                          num_cells_width + 1 + j);
            short v2 = v1 + 1;
            short v3 = v0 + 1;

            v_indexes.push_back(v0);
            v_indexes.push_back(v1);
            v_indexes.push_back(v2);

            v_indexes.push_back(v0);
            v_indexes.push_back(v3);
            v_indexes.push_back(v2);
        }
    }

    TriangleIndexedRender render_triangle_indexed(img);
    render_triangle_indexed.clear(black);

    render_triangle_indexed.draw_triangles_indexed(
        v_vertexes, v_indexes, green);
    std::string file1 = "triangles_indexed.ppm";
    img.save_image(file1);

    return EXIT_SUCCESS;
}
