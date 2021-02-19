#include "triangle_interpolated_render.hxx"

// simple load image
class simple_gfx_prog : public gfx_program {
public:
  Uniform *uni = nullptr;
  simple_gfx_prog();
  void set_uniform(Uniform &uni) override;
  Vertex vertex_shader(const Vertex &v_in) override;
  canv::Color fragment_shader(const Vertex &v_in) override;
};

// gray color image transformation
class gray_transform_gfx : public simple_gfx_prog {
public:
  gray_transform_gfx();
  canv::Color fragment_shader(const Vertex &v_in) override;
};

class cube_gfx_prog : public simple_gfx_prog {
public:
  cube_gfx_prog();
  Vertex vertex_shader(const Vertex &v_in) override;
  canv::Color fragment_shader(const Vertex &v_in) override;
};
