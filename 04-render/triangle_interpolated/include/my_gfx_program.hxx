#include "triangle_interpolated_render.hxx"

class my_gfx_program : public gfx_program
{
public:
    void        set_uniform(Uniform& uni) override;
    Vertex      vertex_shader(const Vertex& v_in) override;
    canv::Color fragment_shader(const Vertex& v_in) override;
};
