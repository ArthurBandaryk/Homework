#include "my_gfx_program.hxx"

int main(int /*argc*/, char* /*argv*/[])
{
    canv::Color black = { 0, 0, 0 };

    canv::Image img;

    TriangleInterpolatedRender render{ img };
    my_gfx_program             gfx;
    render.set_gfx_program(gfx);
    render.clear(black);

    Vertex s{ 0., 0., 255, 0, 0, 0 };
    Vertex f{ static_cast<double>(canv::width - 1.),
              double(canv::height - 1.),
              0,
              255,
              255,
              0 };
    Vertex s_ = gfx.vertex_shader(s);

    Vertex f_ = gfx.vertex_shader(f);

    render.draw_raster_line(s_, f_);

    img.save_image("line_gradient.ppm");

    std::vector<Vertex> triangle_v1{ { 0., 0., 255, 0, 0, 0 },
                                     { 799., 0., 0, 255, 0, 0 },
                                     {
                                         799.,
                                         599.,
                                         0,
                                         0,
                                         255,
                                         0,
                                     } };
    std::vector<short>  indexes_v1{ 0, 1, 2 };

    render.clear(black);
    render.draw_triangles_interpolated(triangle_v1, indexes_v1);

    img.save_image("triangle_gradient1.ppm");

    std::vector<Vertex> triangle_v2{ { 0., 0., 255, 0, 0, 0 },
                                     { 0., 599., 0, 255, 0, 0 },
                                     {
                                         799.,
                                         599.,
                                         0,
                                         0,
                                         255,
                                         0,
                                     } };
    std::vector<short>  indexes_v2{ 0, 1, 2 };

    render.clear(black);
    render.draw_triangles_interpolated(triangle_v2, indexes_v2);

    img.save_image("triangle_gradient2.ppm");

    std::vector<Vertex> triangle_v3{ { 300., 80., 255, 0, 0, 0 },
                                     { 799., 356., 0, 255, 0, 0 },
                                     {
                                         10.,
                                         599.,
                                         0,
                                         0,
                                         255,
                                         0,
                                     } };
    std::vector<short>  indexes_v3{ 0, 1, 2 };

    render.clear(black);
    render.draw_triangles_interpolated(triangle_v3, indexes_v3);

    img.save_image("triangle_gradient3.ppm");

    std::vector<Vertex> triangle_v4{ { 300., 300., 255, 0, 0, 0 },
                                     { 799., 300., 0, 255, 0, 0 },
                                     {
                                         10.,
                                         300.,
                                         0,
                                         0,
                                         255,
                                         0,
                                     } };
    std::vector<short>  indexes_v4{ 0, 1, 2 };

    render.clear(black);
    render.draw_triangles_interpolated(triangle_v4, indexes_v4);

    img.save_image("triangle_gradient4.ppm");

    std::vector<Vertex> triangle_v5{ { 300., 10., 255, 0, 0, 0 },
                                     { 300., 300., 0, 255, 0, 0 },
                                     {
                                         300.,
                                         500.,
                                         0,
                                         0,
                                         255,
                                         0,
                                     } };
    std::vector<short>  indexes_v5{ 0, 1, 2 };

    render.clear(black);
    render.draw_triangles_interpolated(triangle_v5, indexes_v5);

    img.save_image("triangle_gradient5.ppm");

    return EXIT_SUCCESS;
}
