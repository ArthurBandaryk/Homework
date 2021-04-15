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
public:
    Brick(const std::string& path);
    ~Brick();
};

class Ball : public GameObject
{
private:
public:
    Ball(const std::string& path);
    ~Ball();
};

class Life : public GameObject
{
private:
public:
    Life(const std::string& path);
    ~Life();
};

class Platform : public GameObject
{
private:
public:
    Platform(const std::string& path);
    ~Platform();
};

#endif // GAME_COMPONENTS_H
