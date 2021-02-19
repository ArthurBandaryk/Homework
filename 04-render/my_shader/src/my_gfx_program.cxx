#include "my_gfx_program.hxx"
#include "matrix.hxx"

//-------------------------------------------
//-------------------------------------------
//------------simple load image--------------

simple_gfx_prog::simple_gfx_prog() {}

void simple_gfx_prog::set_uniform(Uniform& uni)
{
    this->uni = &uni;
}

Vertex simple_gfx_prog::vertex_shader(const Vertex& v_in)
{
    Vertex v_out = v_in;
    return v_out;
}

canv::Color simple_gfx_prog::fragment_shader(const Vertex& v_in)
{
    size_t index_x = static_cast<size_t>(v_in.u * (uni->img->get_width() - 1));
    size_t index_y = static_cast<size_t>(v_in.v * (uni->img->get_height() - 1));

    canv::Color col;
    col = uni->img->get_pixel(index_x, index_y);

    return col;
}

//-------------------------------------------
//-------------------------------------------
//---------------gray pixeled image----------
//-------------------------------------------
//-------------------------------------------

gray_transform_gfx::gray_transform_gfx() {}

canv::Color gray_transform_gfx::fragment_shader(const Vertex& v_in)
{
    size_t index_x = static_cast<size_t>(v_in.u * (uni->img->get_width() - 1));
    size_t index_y = static_cast<size_t>(v_in.v * (uni->img->get_height() - 1));

    canv::Color col;
    col = uni->img->get_pixel(index_x, index_y);

    uint8_t mid = (0.2125 * col.r) + (0.7154 * col.g) + (0.0721 * col.b);
    col.r       = mid;
    col.g       = mid;
    col.b       = mid;
    return col;
}

//-------------------------------------------
//-------------------------------------------

//-------------------------------------------
//-------------------------------------------
//---------------------rotating cube------------------
cube_gfx_prog::cube_gfx_prog() {}

Vertex cube_gfx_prog::vertex_shader(const Vertex& v_in)
{
    Vertex out = v_in;

    // Matrix m;
    // m.set_scale_matrix(1.f, 1.f, 1.f);
    // Vector v1{ static_cast<float>(v_in.x), static_cast<float>(v_in.y), 0.f };
    // v1 = m.multiply_vect(v1);
    // m.set_rotateX_matrix(0.f);
    // v1 = m.multiply_vect(v1);
    // m.set_rotateY_matrix(0.f);
    // v1 = m.multiply_vect(v1);
    // m.set_rotateZ_matrix(40.f);
    // v1 = m.multiply_vect(v1);

    // Vector v2{ static_cast<float>(v_in.u), static_cast<float>(v_in.v), 0.f };
    out.x -= 320 / 2;
    out.y -= 240 / 2;
    double x = out.x;
    double y = out.y;
    // std::cout << "x = " << out.x << " y = " << out.y << std::endl;
    double alfa = (3.14159 / 180.) * uni->time;
    std::cout << uni->time << std::endl;
    std::cout << alfa << std::endl;
    out.x = x * std::cos(alfa) - y * std::sin(alfa);
    out.y = x * std::sin(alfa) + y * std::cos(alfa);
    out.x += (320 / 2);
    out.y += (240 / 2);
    // std::cout << "x = " << out.x << " y = " << out.y << std::endl;
    return out;
}

canv::Color cube_gfx_prog::fragment_shader(const Vertex& v_in)
{
    canv::Color out;
    out.r = v_in.r;
    out.g = v_in.g;
    out.b = v_in.b;
    return out;
}
//-------------------------------------------
//-------------------------------------------
