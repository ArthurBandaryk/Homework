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

void Platform::set_matrix(const Matrix& m)
{
    if (vertexes[0].v_pos.x <= -1.f ||
        vertexes[0].v_pos.x >= 1.f)
        return;
    this->m = this->m * m;
}

Matrix Platform::get_matrix() const
{
    return m;
}

float Platform::get_speed() const
{
    return speed;
}
