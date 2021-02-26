#pragma once
#include <array>

namespace eng {

enum class Events {
  left_pressed,
  left_released,
  right_pressed,
  right_released,
  up_pressed,
  up_released,
  down_pressed,
  down_released,

  select_pressed,
  select_released,
  start_pressed,
  start_released,

  button1_X_pressed,
  button1_X_released,
  button2_A_pressed,
  button2_A_released,
  button3_Y_pressed,
  button3_Y_released,
  button4_B_pressed,
  button4_B_released,

  mouse_left_pressed,
  mouse_left_released,
  mouse_right_pressed,
  mouse_right_released,

  turn_off
};

struct Vertex {
  float x;
  float y;
  float z;
  float r;
  float g;
  float b;
  Vertex();
  // clang-format off
  Vertex(float x, float y, float z,
         float r, float g, float b);
  // clang-format on
};

struct Triangle {
  std::array<Vertex, 3> triangle;
  Triangle();
  // clang-format off
  Triangle(const Vertex &v1,
           const Vertex &v2,
           const Vertex &v3);

  // clang-format on
};

class IEngine {
private:
public:
  virtual ~IEngine();
  virtual int init() = 0;
  virtual void uninit() = 0;
  virtual bool read_input(eng::Events &ev) = 0;
  virtual void render_triangle(const Triangle &v) = 0;
  virtual void swap_buffers() = 0;
  virtual void update() = 0;
};

class Engine_impl : public eng::IEngine {
public:
  int init() override;
  void uninit() override;
  bool read_input(eng::Events &ev) override;
  void render_triangle(const Triangle &t) override;
  void swap_buffers() override;
  void update() override;
};

IEngine *create_engine();
void destroy_engine(IEngine *&);

} // namespace eng
