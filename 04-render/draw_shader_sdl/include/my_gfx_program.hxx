#include "triangle_interpolated_render.hxx"

class my_gfx_program : public gfx_program {
private:
  double r;
  double x_mouse;
  double y_mouse;

public:
  my_gfx_program();
  void set_uniform(Uniform &uni) override;
  Vertex vertex_shader(const Vertex &v_in) override;
  canv::Color fragment_shader(const Vertex &v_in) override;
  void set_mouse_coordinates(const double x, const double y);
  void set_radius(const double r);
  double get_radius() const;
  double get_mouse_x() const;
  double get_mouse_y() const;
};
