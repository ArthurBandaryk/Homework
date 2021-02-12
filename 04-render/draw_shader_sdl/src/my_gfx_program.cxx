#include "my_gfx_program.hxx"
#include <algorithm>

my_gfx_program::my_gfx_program()
{
    x_mouse = y_mouse = r = 0.;
}

void my_gfx_program::set_uniform(Uniform& uni)
{
    x_mouse = uni.x;
    y_mouse = uni.y;
    r       = static_cast<double>(uni.reserve);
}

Vertex my_gfx_program::vertex_shader(const Vertex& v_in)
{
    Vertex v_out = v_in;
    double x     = v_out.x;
    double y     = v_out.y;

    double dx = x - x_mouse;
    double dy = y - y_mouse;

    if (dx * dx + dy * dy < r * r)
    {
        double d = std::sqrt(dx * dx + dy * dy);
        if (d > 0)
        {
            // normalize vector from vertex to mouse pos
            double dx_n = dx / d;
            double dy_n = dy / d;
            // find position of point on radius from mouse pos in center
            double radius_pos_x = x_mouse + dx_n * r;
            double radius_pos_y = y_mouse + dy_n * r;
            // find middle point
            x = (x + radius_pos_x) / 2;
            y = (y + radius_pos_y) / 2;
        }
    }

    v_out.x = x;
    v_out.y = y;
    if (v_out.x < 0)
        v_out.x = 0.;
    if (v_out.y < 0)
        v_out.y = 0.;
    if (v_out.x >= static_cast<double>(canv::width))
        v_out.x = static_cast<double>(canv::width - 1);
    if (v_out.y >= static_cast<double>(canv::height))
        v_out.y = static_cast<double>(canv::height - 1);
    return v_out;
}

canv::Color my_gfx_program::fragment_shader(const Vertex& v_in)
{
    canv::Color col;
    col.r = v_in.r;
    col.g = v_in.g;
    col.b = v_in.b;

    double x  = v_in.x;
    double y  = v_in.y;
    double dx = x_mouse - x;
    double dy = y_mouse - y;
    if (dx * dx + dy * dy < r * r)
    {
        double  d = std::sqrt(dx * dx + dy * dy);
        uint8_t col_spec =
            static_cast<uint8_t>((d / r) * static_cast<uint8_t>(255));
        col_spec = std::clamp(
            col_spec, static_cast<uint8_t>(0), static_cast<uint8_t>(255));
        col.r = (255 - col_spec);
        col.g = col_spec;
        col.b = 0;
    }
    return col;
}
