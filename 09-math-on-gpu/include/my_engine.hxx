#pragma once
#include "glad/glad.h"
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

struct Vector
{
    float x;
    float y;
    Vector();
    Vector(float x, float y);
    friend std::ostream& operator<<(std::ostream& os,
                                    const Vector& v);
};

struct Vertex1
{
    Vector v_pos;
    Vertex1();
    Vertex1(const Vector v);
};

struct Vertex2
{
    Vector v_pos;
    Vector v_texture;
    Vertex2();
    // clang-format off
    Vertex2(Vector v_pos, Vector v_texture);
    // clang-format on
};

struct Triangle1
{
    std::array<Vertex1, 3> triangle;
    Triangle1();
    // clang-format off
    Triangle1(const Vertex1 &v1,
           const Vertex1 &v2,
           const Vertex1 &v3);

    // clang-format on
};

struct Triangle2
{
    std::array<Vertex2, 3> triangle;
    Triangle2();
    // clang-format off
    Triangle2(const Vertex2 &v1,
           const Vertex2 &v2,
           const Vertex2 &v3);

    // clang-format on
};

struct Matrix
{
    Vector               col1;
    Vector               col2;
    Vector               col3;
    static Matrix        rotate(float alfa); // alfa in degrees
    static Matrix        move(const Vector& v);
    static Matrix        scale(float scale);
    friend std::ostream& operator<<(std::ostream& os,
                                    const Matrix& m);
};

Vector operator*(const Vector& v, const Matrix& m);
Matrix operator*(const Matrix& m1, const Matrix& m2);

class Texture;

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
        const std::string_view path)               = 0;
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
    Texture* create_texture(
        const std::string_view path) override;
    void destroy_texture(Texture* texture) override;
};

class Texture
{
public:
    virtual ~Texture();
    virtual GLuint get_texture_id() const = 0;
};

class TextureTank : public Texture
{
private:
    std::string path;
    GLuint      texture_id = 0;
    GLuint      width;
    GLuint      height;

public:
    ~TextureTank() override;
    TextureTank(const std::string_view path);
    GLuint get_texture_id() const override;
    GLuint get_texture_width() const;
    GLuint get_texture_height() const;
};

IEngine* create_engine();
void     destroy_engine(IEngine*&);

} // namespace eng
