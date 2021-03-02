#pragma once
#include <array>
#include <chrono>

namespace eng
{

enum class Events
{
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

struct Vertex
{
    float x;
    float y;
    float tx;
    float ty;
    Vertex();
    // clang-format off
    Vertex(float x,  float y,
           float tx, float ty);
    // clang-format on
};

struct Triangle
{
    std::array<Vertex, 3> triangle;
    Triangle();
    // clang-format off
  Triangle(const Vertex &v1,
           const Vertex &v2,
           const Vertex &v3);

    // clang-format on
};

Vertex   interpolate(const Vertex& v1,
                     const Vertex& v2,
                     const float   t);
Triangle interpolate(const Triangle& tr1,
                     const Triangle& tr2,
                     const float     t);

class IEngine
{

public:
    virtual ~IEngine();
    virtual int   init()                              = 0;
    virtual void  uninit()                            = 0;
    virtual bool  read_input(eng::Events& ev)         = 0;
    virtual void  render_triangle(const Triangle& v)  = 0;
    virtual void  swap_buffers()                      = 0;
    virtual void  update()                            = 0;
    virtual float get_time_from_init()                = 0;
    virtual bool  load_texture(std::string_view path) = 0;
};

class Engine_impl : public eng::IEngine
{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock>
        time_begin;

public:
    int   init() override;
    void  uninit() override;
    bool  read_input(eng::Events& ev) override;
    void  render_triangle(const Triangle& t) override;
    void  swap_buffers() override;
    void  update() override;
    float get_time_from_init() override;
    bool  load_texture(std::string_view path) override;
};

IEngine* create_engine();
void     destroy_engine(IEngine*&);

} // namespace eng
