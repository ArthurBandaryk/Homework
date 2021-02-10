#include "triangle_basic_render.hxx"

int main(int /*argc*/, char* /*argv*/[])
{
    canv::Color    black = { 0, 0, 0 };
    canv::Color    green = { 0, 255, 0 };
    canv::Image    img;
    TriangleRender render{ img };
    render.clear(black);
    std::vector<canv::Position> triangle;
    triangle.emplace_back(canv::Position{ 0, 0 });
    triangle.emplace_back(canv::Position{ canv::width - 1, canv::height - 1 });
    triangle.emplace_back(canv::Position{ 0, canv::height - 1 });
    render.draw_triangles(triangle, 3, green);
    std::string file1 = "triangle.ppm";
    img.save_image(file1);

    render.clear(black);
    constexpr size_t num_triangles_raw = 10;
    constexpr size_t num_triangles_col = 10;
    constexpr size_t width_triangle    = 50;
    constexpr size_t height_triangle   = 50;

    std::vector<canv::Position> triangles;
    for (size_t i = 0; i < num_triangles_raw; ++i)
    {
        for (size_t j = 0; j < num_triangles_col; j++)
        {
            canv::Position v0{ static_cast<int32_t>(i * width_triangle),
                               static_cast<int32_t>(j * height_triangle) };
            canv::Position v1{ static_cast<int32_t>(i * width_triangle),
                               static_cast<int32_t>(height_triangle +
                                                    j * height_triangle) };
            canv::Position v2{
                static_cast<int32_t>(i * width_triangle + width_triangle),
                static_cast<int32_t>(height_triangle + j * height_triangle)
            };
            canv::Position v3{ static_cast<int32_t>(width_triangle +
                                                    i * width_triangle),
                               static_cast<int32_t>(j * height_triangle) };

            triangles.push_back(v0);
            triangles.push_back(v1);
            triangles.push_back(v2);

            triangles.push_back(v0);
            triangles.push_back(v3);
            triangles.push_back(v2);
        }
    }
    render.draw_triangles(triangles, triangles.size(), green);
    std::string file2 = "triangles.ppm";
    img.save_image(file2);

    return EXIT_SUCCESS;
}
