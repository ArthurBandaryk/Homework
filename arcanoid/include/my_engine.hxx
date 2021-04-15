#pragma once
#include "glad/glad.h"
#include "my_math.hxx"
#include "texture.hxx"
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
    mouse_motion,
    unknown,

    turn_off
};

class IEngine
{

public:
    virtual ~IEngine();
    virtual int      init()                              = 0;
    virtual void     uninit()                            = 0;
    virtual bool     read_input(eng::Events& ev)         = 0;
    virtual void     render_triangle(const Triangle1& t) = 0;
    virtual void     render_triangle(const Triangle2& t,
                                     Texture*         texture)   = 0;
    virtual void     render_triangle(const Triangle2& t,
                                     Texture*         texture,
                                     const Matrix&    m)    = 0;
    virtual void     swap_buffers()                      = 0;
    virtual void     update()                            = 0;
    virtual float    get_time_from_init()                = 0;
    virtual Texture* create_texture(
        const std::string& path)                   = 0;
    virtual void destroy_texture(Texture* texture) = 0;
};

class Engine_impl : public eng::IEngine
{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock>
        time_begin;

public:
    int  init() override;
    void uninit() override;
    bool read_input(eng::Events& ev) override;
    void render_triangle(const Triangle1& t) override;
    void render_triangle(const Triangle2& t,
                         Texture*         texture) override;
    void render_triangle(const Triangle2& t,
                         Texture*         texture,
                         const Matrix&    m) override;

    void     swap_buffers() override;
    void     update() override;
    float    get_time_from_init() override;
    Texture* create_texture(const std::string& path) override;
    void     destroy_texture(Texture* texture) override;
};

class VertexBuffer
{
};

class IndexBuffer
{
};

IEngine* create_engine();
void     destroy_engine(IEngine*&);

} // namespace eng
