#pragma once

#include <chrono>
#include <list>
#include <memory>
#include <vector>

#include "game_components.hxx"
#include "my_engine.hxx"

enum ButtonNames
{
    left,
    right
};

struct Button
{
    ButtonNames name;
    bool        is_pressed;
};

class Arcanoid
{
private:
    std::unique_ptr<Texture> background;
    std::unique_ptr<Texture> health_texture;
    std::unique_ptr<Texture> ball_texture;
    std::unique_ptr<Texture> platform_texture;
    std::vector<Texture*>    v_bricks_textures;

    std::vector<Brick>        bricks;
    std::unique_ptr<Ball>     ball;
    std::unique_ptr<Platform> platform;
    std::vector<Life*>        health;

    eng::IEngine*         engine;
    std::list<Wall>       walls;
    std::array<Button, 2> controls;
    size_t                current_level = 1;
    bool                  is_running    = false;

    void init_background();
    void init_walls();
    void init_bricks();
    void init_ball();
    void init_platform();
    void init_health();
    void render_background() const;
    void render_walls();
    void render_bricks() const;
    void render_ball() const;
    void render_health() const;
    void render_platform() const;
    void platform_move(DirectionPlatform dir,
                       float             delta_time_frame);
    void ball_move(float delta_time_frame);
    void collision_process();

public:
    Arcanoid();
    ~Arcanoid();
    size_t game_init();
    void   game_event();
    void   game_update(float delta_time_frame);
    void   game_render();
    void   game_uninit();
    bool   game_running() const;
};
