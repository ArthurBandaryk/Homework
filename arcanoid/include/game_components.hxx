#ifndef GAME_COMPONENTS_H
#define GAME_COMPONENTS_H

#include "my_engine.hxx"
#include <array>
#include <memory>

enum class WallType
{
    left,
    up,
    right
};

enum class BrickType
{
    yellow,
    orange,
    orange_broken,
    green,
    green_broken,
    dark_green,
    dark_green_broken,
    blue,
    blue_broken,
    dark_blue,
    dark_blue_broken,
    purple,
    purple_broken,
    red,
    red_broken,
    marron,
    marron_broken,
    gray,
    gray_broken,
};

class GameObject
{
protected:
    float                  width;
    float                  height;
    std::array<Vertex2, 4> vertexes;
    std::string            path;

public:
    GameObject();
    ~GameObject();
    GameObject(const std::string& path);
    Vertex2 get_vertex(const size_t index) const;
    void    set_vertex(const size_t index, const Vertex2& v);
    float   get_width() const;
    float   get_height() const;
    std::string get_path() const;
};

class Wall : public GameObject
{
    std::array<Vertex1, 4> vertexes;
    WallType               type;
    // (v1) ---------------- (v4)
    // |                        |
    // |                        |
    // |                        |
    // |                        |
    // (v2) ---------------- (v3)

public:
    ~Wall();
    Wall(const Vertex1& v1,
         const Vertex1& v2,
         const Vertex1& v3,
         const Vertex1& v4,
         WallType       type);
    Vertex1 operator[](const size_t index);
};

class Brick : public GameObject
{
private:
    size_t cnt_hit = 0; // number of ball's hits to brick
    size_t status_hit_fatal = 1;
    // if cnt_hit = status_hit_fatal => next hit of the
    // ball will destroy brick
    bool is_destroyed = false;

public:
    Brick(const std::string& path);
    ~Brick();
    void destroy(bool flag);
    bool destroyed() const;
};

class Ball : public GameObject
{
private:
    Vector direction{ 0.4f, 1.5f };
    Vector speed_max{ 1.3f, 1.5f };

    Triangle2 tr_low;
    Triangle2 tr_high;
    bool      is_destroyed = false;

public:
    Ball(const std::string& path);
    ~Ball();
    void       set_dir(const float x, const float y);
    Vector     get_dir() const;
    Vector     get_speed_max() const;
    Triangle2& get_triangle_low();
    Triangle2& get_triangle_high();
};

class Life : public GameObject
{
private:
public:
    Life(const std::string& path);
    ~Life();
};

enum class DirectionPlatform
{
    left,
    right
};

class Platform : public GameObject
{
private:
    Matrix m     = Matrix::scale(1.f);
    float  speed = 1.5f;

public:
    Platform(const std::string& path);
    ~Platform();
    float get_speed() const;
};

#endif // GAME_COMPONENTS_H
