#include "triangle_interpolated_render.hxx"
#include <algorithm>

gfx_program::~gfx_program() {}

double interpolate(double x0, double x1, double t)
{
    return (x1 - x0) * t + x0;
}

Vertex interpolate(const Vertex& v0, const Vertex& v1, double t)
{
    Vertex result{
        interpolate(v0.x, v1.x, t),
        interpolate(v0.y, v1.y, t),
        static_cast<uint8_t>(interpolate(
            static_cast<double>(v0.r), static_cast<double>(v1.r), t)),
        static_cast<uint8_t>(interpolate(
            static_cast<double>(v0.g), static_cast<double>(v1.g), t)),
        static_cast<uint8_t>(interpolate(
            static_cast<double>(v0.b), static_cast<double>(v1.b), t)),
        static_cast<uint8_t>(interpolate(static_cast<double>(v0.reserve),
                                         static_cast<double>(v1.reserve),
                                         t))
    };
    return result;
}

TriangleInterpolatedRender::TriangleInterpolatedRender(canv::Image& img)
    : TriangleIndexedRender(img)
{
}

void TriangleInterpolatedRender::set_gfx_program(gfx_program& prog)
{
    this->prog = &prog;
}

void TriangleInterpolatedRender::raster_one_horizontal_line(
    const Vertex&        left_vertex,
    const Vertex&        right_vertex,
    std::vector<Vertex>& v_out)
{
    canv::Position start{ static_cast<int32_t>(left_vertex.x),
                          static_cast<int32_t>(left_vertex.y) };
    canv::Position finish{ static_cast<int32_t>(right_vertex.x),
                           static_cast<int32_t>(right_vertex.y) };

    // canv::pixels v_pixs_coords = pixels_positions(start, finish);

    size_t num_pixels = static_cast<size_t>(
        std::round(std::fabs(left_vertex.x - right_vertex.x)));

    // size_t num_pixels = v_pixs_coords.size();

    if (num_pixels > 0)
    {
        for (size_t i = 0; i <= num_pixels + 1; ++i)
        {
            double t =
                (static_cast<double>(i) / static_cast<double>(num_pixels + 1));
            Vertex result = interpolate(left_vertex, right_vertex, t);
            v_out.push_back(result);
        }
    }
    else
    {
        v_out.push_back(left_vertex);
    }
}

std::vector<Vertex> TriangleInterpolatedRender::raster_horizontal_triangle(
    const Vertex& top, const Vertex& left_line, const Vertex& right_line)
{
    std::vector<Vertex> v_results;

    size_t num_lines_vertical =
        static_cast<size_t>(std::round(fabs(top.y - left_line.y)));

    if (num_lines_vertical > 0)
    {
        for (size_t i = 0; i <= num_lines_vertical; ++i)
        {
            double t = static_cast<double>(i) /
                       static_cast<double>(num_lines_vertical);
            Vertex left  = interpolate(left_line, top, t);
            Vertex right = interpolate(right_line, top, t);
            raster_one_horizontal_line(left, right, v_results);
        }
    }
    else
    {
        raster_one_horizontal_line(left_line, right_line, v_results);
    }

    return v_results;
}

std::vector<Vertex> TriangleInterpolatedRender::rasterize_triangle(
    const Vertex& v0, const Vertex& v1, const Vertex& v2)
{
    std::vector<Vertex> v_results;

    std::array<const Vertex*, 3> arr_vertexes{ &v0, &v1, &v2 };

    std::sort(arr_vertexes.begin(),
              arr_vertexes.end(),
              [](const Vertex* const l, const Vertex* const r) -> bool {
                  return l->y < r->y;
              });

    const Vertex& top    = *arr_vertexes.at(0);
    const Vertex& middle = *arr_vertexes.at(1);
    const Vertex& bottom = *arr_vertexes.at(2);

    canv::Position p_top{ static_cast<int32_t>(std::round(top.x)),
                          static_cast<int32_t>(std::round(top.y)) };
    canv::Position p_mid{ static_cast<int32_t>(std::round(middle.x)),
                          static_cast<int32_t>(std::round(middle.y)) };
    canv::Position p_bot{ static_cast<int32_t>(std::round(bottom.x)),
                          static_cast<int32_t>(std::round(bottom.y)) };

    if (p_top == p_mid)
    {
        if (p_top == p_bot)
        { // it's a point
            v_results.push_back(top);
            v_results.push_back(middle);
            v_results.push_back(bottom);
            return v_results;
        }
        else
        { // it's a line
            raster_one_horizontal_line(bottom, middle, v_results);
            return v_results;
        }
    }

    if (p_top.y == p_mid.y)
    {
        std::vector<Vertex> temp =
            raster_horizontal_triangle(bottom, top, middle);
        for (const auto& el : temp)
        {
            v_results.push_back(el);
        }
        return v_results;
    }

    if (p_mid.y == p_bot.y)
    {
        std::vector<Vertex> temp =
            raster_horizontal_triangle(top, middle, bottom);
        for (const auto& el : temp)
        {
            v_results.push_back(el);
        }
        return v_results;
    }

    canv::pixels longest_side = pixels_positions(p_top, p_bot);

    auto it_mid = std::find_if(longest_side.begin(),
                               longest_side.end(),
                               [&middle](const canv::Position& pos) -> bool {
                                   return pos.y == static_cast<int32_t>(
                                                       std::round(middle.y));
                               });
    if (it_mid == longest_side.end())
    {
        std::cerr << "Cannot find mid point on the longest side" << std::endl;
        return v_results;
    }
    else
    {
        canv::Position p_mid_2 = *it_mid;
        double         h_lines =
            static_cast<double>(std::round(fabs(top.y - bottom.y)));
        double ind = static_cast<double>(std::round(fabs(top.y - p_mid_2.y)));
        double t   = ind / h_lines;
        Vertex middle2{
            static_cast<double>(p_mid_2.x),
            static_cast<double>(p_mid_2.y),
            static_cast<uint8_t>(interpolate(
                static_cast<double>(top.r), static_cast<double>(bottom.r), t)),
            static_cast<uint8_t>(interpolate(
                static_cast<double>(top.g), static_cast<double>(bottom.g), t)),
            static_cast<uint8_t>(interpolate(
                static_cast<double>(top.b), static_cast<double>(bottom.b), t)),
            static_cast<uint8_t>(
                interpolate(static_cast<double>(top.reserve),
                            static_cast<double>(bottom.reserve),
                            t))
        };

        std::vector<Vertex> high_triangle =
            raster_horizontal_triangle(top, middle2, middle);
        std::vector<Vertex> low_triangle =
            raster_horizontal_triangle(bottom, middle2, middle);

        v_results.insert(
            v_results.end(), high_triangle.begin(), high_triangle.end());
        v_results.insert(
            v_results.end(), low_triangle.begin(), low_triangle.end());
    }

    return v_results;
}

void TriangleInterpolatedRender::draw_raster_line(const Vertex& left,
                                                  const Vertex& right)
{
    std::vector<Vertex> result_draw;
    raster_one_horizontal_line(left, right, result_draw);
    for (auto& el : result_draw)
    {
        canv::Position pixel{ static_cast<int32_t>(el.x),
                              static_cast<int32_t>(el.y) };
        canv::Color    col = prog->fragment_shader(el);
        this->set_pixel(pixel, col);
    }
}

void TriangleInterpolatedRender::draw_triangles_interpolated(
    std::vector<Vertex>& v_vertexes, std::vector<short>& v_indexes)
{
    size_t num_indexes = v_indexes.size();

    for (size_t i = 0; i < num_indexes; i += 3)
    {
        short i0 = v_indexes.at(i);
        short i1 = v_indexes.at(i + 1);
        short i2 = v_indexes.at(i + 2);

        Vertex v0 = v_vertexes.at(i0);
        Vertex v1 = v_vertexes.at(i1);
        Vertex v2 = v_vertexes.at(i2);

        Vertex v0_interp = prog->vertex_shader(v0);
        Vertex v1_interp = prog->vertex_shader(v1);
        Vertex v2_interp = prog->vertex_shader(v2);

        std::vector<Vertex> v_vertexes_interp{ rasterize_triangle(
            v0_interp, v1_interp, v2_interp) };

        for (const auto& el : v_vertexes_interp)
        {
            canv::Color    col = prog->fragment_shader(el);
            canv::Position pos{ static_cast<int32_t>(std::round(el.x)),
                                static_cast<int32_t>(std::round(el.y)) };
            this->set_pixel(pos, col);
        }
    }
}
