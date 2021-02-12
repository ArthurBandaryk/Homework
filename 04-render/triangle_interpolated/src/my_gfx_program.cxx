#include "my_gfx_program.hxx"

void my_gfx_program::set_uniform(Uniform& uni)
{
    return;
}

Vertex my_gfx_program::vertex_shader(const Vertex& v_in)
{
    Vertex v_out = v_in;
    return v_out;
}

canv::Color my_gfx_program::fragment_shader(const Vertex& v_in)
{
    canv::Color col;
    col.r = v_in.r;
    col.g = v_in.g;
    col.b = v_in.b;
    return col;
}
