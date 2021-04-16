#include "game_components.hxx"
#include <iostream>

GameObject::~GameObject() {}

GameObject::GameObject() {}

GameObject::GameObject(const std::string& path)
{
    this->path = path;
}

Vertex2 GameObject::get_vertex(const size_t index) const
{
    return vertexes[index];
}

void GameObject::set_vertex(const size_t   index,
                            const Vertex2& v)
{
    vertexes[index] = v;
}

float GameObject::get_width() const
{
    return width;
}

float GameObject::get_height() const
{
    return height;
}

std::string GameObject::get_path() const
{
    return path;
}
//-------------------Wall--------------------------

static size_t window_width  = 1024; // pixels
static size_t window_height = 768;  // pixels

extern float wall_width_left;
extern float wall_width_right;
extern float wall_height_up;

Wall::~Wall() {}

Wall::Wall(const Vertex1& v1,
           const Vertex1& v2,
           const Vertex1& v3,
           const Vertex1& v4,
           WallType       type)
{
    vertexes[0] = v1;
    vertexes[1] = v2;
    vertexes[2] = v3;
    vertexes[3] = v4;
    this->type  = type;
}

Vertex1 Wall::operator[](const size_t index)
{
    return vertexes[index];
}

//-------------------Brick-------------------------

Brick::~Brick() {}

Brick::Brick(const std::string& path)
    : GameObject(path)
{
    width  = 2.f / 10.f;
    height = 1.5f * wall_height_up;
}

void Brick::destroy(bool flag)
{
    this->is_destroyed = flag;
}

bool Brick::destroyed() const
{
    return is_destroyed;
}

//-------------------Ball-------------------------

Ball::~Ball() {}

Ball::Ball(const std::string& path)
    : GameObject(path)
{
    width  = 20.f * 2.f / static_cast<float>(window_width);
    height = 20.f * 2.f / static_cast<float>(window_height);

    vertexes[0].v_pos.x     = 0.f - width / 2.f;
    vertexes[0].v_pos.y     = 0.f + height / 2.f;
    vertexes[0].v_texture.x = 0.f;
    vertexes[0].v_texture.y = 1.f;

    vertexes[1].v_pos.x     = 0.f - width / 2.f;
    vertexes[1].v_pos.y     = 0.f - height / 2.f;
    vertexes[1].v_texture.x = 0.f;
    vertexes[1].v_texture.y = 0.f;

    vertexes[2].v_pos.x     = 0.f + width / 2.f;
    vertexes[2].v_pos.y     = 0.f - height / 2.f;
    vertexes[2].v_texture.x = 1.f;
    vertexes[2].v_texture.y = 0.f;

    vertexes[3].v_pos.x     = 0.f + width / 2.f;
    vertexes[3].v_pos.y     = 0.f + height / 2.f;
    vertexes[3].v_texture.x = 1.f;
    vertexes[3].v_texture.y = 1.f;

    tr_low.triangle[0] = vertexes[0];
    tr_low.triangle[1] = vertexes[1];
    tr_low.triangle[2] = vertexes[2];

    tr_high.triangle[0] = vertexes[0];
    tr_high.triangle[1] = vertexes[3];
    tr_high.triangle[2] = vertexes[2];
}

void Ball::set_dir(const float x, const float y)
{
    direction.x = x;
    direction.y = y;
}

Vector Ball::get_dir() const
{
    return direction;
}

Vector Ball::get_speed_max() const
{
    return speed_max;
}

Triangle2& Ball::get_triangle_low()
{
    return tr_low;
}

Triangle2& Ball::get_triangle_high()
{
    return tr_high;
}

//-------------------Life-------------------------

Life::~Life() {}

Life::Life(const std::string& path)
    : GameObject(path)
{
    width  = 40.f * 2.f / static_cast<float>(window_width);
    height = 40.f * 2.f / static_cast<float>(window_height);
}

//-------------------Platform----------------------

Platform::~Platform() {}

Platform::Platform(const std::string& path)
    : GameObject(path)
{
    width  = 130.f * 2.f / static_cast<float>(window_width);
    height = 20.f * 2.f / static_cast<float>(window_height);
}

float Platform::get_speed() const
{
    return speed;
}
