#include <algorithm>
#include <chrono>
#include <iostream>

#include "game.hxx"
//#include "game_components.hxx"

static float window_width  = 1024.f; // pixels
static float window_height = 768.f;  // pixels

float wall_width_left  = 20.f * 2.f / window_width;
float wall_height_up   = 20.f * 2.f / window_height;
float wall_width_right = wall_width_left;

Arcanoid::Arcanoid() {}

Arcanoid::~Arcanoid()
{
    for (auto& el : v_bricks_textures)
    {
        delete el;
    }

    for (auto& el : health)
    {
        delete el;
    }
}

bool Arcanoid::game_running() const
{
    return is_running;
}

void Arcanoid::init_background()
{
    std::string background_path =
        "resources/backgrounds/background3.png";

    background = std::make_unique<Texture>(background_path);
}

void Arcanoid::init_walls()
{
    Wall w_left{
        Vertex1{ Vector{ -1.f, 1.f } },
        Vertex1{ Vector{ -1.f, -1.f } },
        Vertex1{ Vector{ -1.f + wall_width_left, -1.f } },
        Vertex1{ Vector{ -1.f + wall_width_left, 1.f } },
        WallType::left
    };

    Wall w_up{ Vertex1{ Vector{ -1.f, 1.f } },
               Vertex1{ Vector{ -1.f, 1.f - wall_height_up } },
               Vertex1{ Vector{ 1.f, 1.f - wall_height_up } },
               Vertex1{ Vector{ 1.f, 1.f } },
               WallType::left };

    Wall w_right{
        Vertex1{ Vector{ 1.f - wall_width_right, 1.f } },
        Vertex1{ Vector{ 1.f - wall_width_right, -1.f } },
        Vertex1{ Vector{ 1.f, -1.f } },
        Vertex1{ Vector{ 1.f, 1.f } },
        WallType::right
    };

    walls.push_back(w_left);
    walls.push_back(w_up);
    walls.push_back(w_right);
}

void Arcanoid::init_bricks()
{
    Texture* brick_yellow_texture =
        new Texture("resources/bricks/yellow_brick.png");

    Texture* brick_gray_texture =
        new Texture("resources/bricks/gray_brick.png");

    Texture* brick_marron_texture =
        new Texture("resources/bricks/marron_brick.png");

    Texture* brick_red_texture =
        new Texture("resources/bricks/red_brick.png");

    Texture* brick_purple_texture =
        new Texture("resources/bricks/purple_brick.png");

    Texture* brick_green_texture =
        new Texture("resources/bricks/green_brick.png");

    Texture* brick_dark_green_texture =
        new Texture("resources/bricks/dark_green_brick.png");

    Texture* brick_blue_texture =
        new Texture("resources/bricks/blue_brick.png");

    Texture* brick_dark_blue_texture =
        new Texture("resources/bricks/dark_blue_brick.png");

    Texture* brick_orange_texture =
        new Texture("resources/bricks/orange_brick.png");

    v_bricks_textures.push_back(brick_yellow_texture);
    v_bricks_textures.push_back(brick_gray_texture);
    v_bricks_textures.push_back(brick_marron_texture);
    v_bricks_textures.push_back(brick_red_texture);
    v_bricks_textures.push_back(brick_purple_texture);
    v_bricks_textures.push_back(brick_green_texture);
    v_bricks_textures.push_back(brick_dark_green_texture);
    v_bricks_textures.push_back(brick_blue_texture);
    v_bricks_textures.push_back(brick_dark_blue_texture);
    v_bricks_textures.push_back(brick_orange_texture);

    Brick brick_yellow(brick_yellow_texture->get_path());
    Brick brick_gray(brick_gray_texture->get_path());
    Brick brick_marron(brick_marron_texture->get_path());
    Brick brick_red(brick_red_texture->get_path());
    Brick brick_purple(brick_purple_texture->get_path());
    Brick brick_green(brick_green_texture->get_path());
    Brick brick_dark_green(
        brick_dark_green_texture->get_path());
    Brick brick_blue(brick_blue_texture->get_path());
    Brick brick_dark_blue(brick_dark_blue_texture->get_path());
    Brick brick_orange(brick_orange_texture->get_path());

    Vertex2 v1, v2, v3, v4;

    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            v1.v_pos.x     = -1.f + j * brick_blue.get_width();
            v1.v_pos.y     = 1.f - i * brick_blue.get_height();
            v1.v_texture.x = 0.f;
            v1.v_texture.y = 1.f;

            v2.v_pos.x = -1.f + j * brick_blue.get_width();
            v2.v_pos.y = 1.f - i * brick_blue.get_height() -
                         brick_blue.get_height();
            v2.v_texture.x = 0.f;
            v2.v_texture.y = 0.f;

            v3.v_pos.x = -1.f + j * brick_blue.get_width() +
                         brick_blue.get_width();
            v3.v_pos.y = 1.f - i * brick_blue.get_height() -
                         brick_blue.get_height();
            v3.v_texture.x = 1.f;
            v3.v_texture.y = 0.f;

            v4.v_pos.x = -1.f + j * brick_blue.get_width() +
                         brick_blue.get_width();
            v4.v_pos.y     = 1.f - i * brick_blue.get_height();
            v4.v_texture.x = 1.f;
            v4.v_texture.y = 1.f;

            if (!i || i == 9) // yellow bricks
            {

                brick_yellow.set_vertex(0, v1);
                brick_yellow.set_vertex(1, v2);
                brick_yellow.set_vertex(2, v3);
                brick_yellow.set_vertex(3, v4);
                bricks.push_back(brick_yellow);
            }

            if (i == 1)               // marron bricks
            {                         // i = 1
                if (j == 1 || j == 2) // gray bricks
                {
                    brick_marron.set_vertex(0, v1);
                    brick_marron.set_vertex(1, v2);
                    brick_marron.set_vertex(2, v3);
                    brick_marron.set_vertex(3, v4);
                    bricks.push_back(brick_marron);
                }
                else // marron bricks
                {
                    brick_gray.set_vertex(0, v1);
                    brick_gray.set_vertex(1, v2);
                    brick_gray.set_vertex(2, v3);
                    brick_gray.set_vertex(3, v4);
                    bricks.push_back(brick_gray);
                }
            } // i = 1

            if (i == 2) // red bricks
            {           // i = 2
                if (j == 3 || j == 4 || j == 6 || j == 7 ||
                    j == 9) // red
                {
                    brick_red.set_vertex(0, v1);
                    brick_red.set_vertex(1, v2);
                    brick_red.set_vertex(2, v3);
                    brick_red.set_vertex(3, v4);
                    bricks.push_back(brick_red);
                }
                else // gray
                {
                    brick_marron.set_vertex(0, v1);
                    brick_marron.set_vertex(1, v2);
                    brick_marron.set_vertex(2, v3);
                    brick_marron.set_vertex(3, v4);
                    bricks.push_back(brick_marron);
                }
            } // i = 2
            if (i == 3)
            {                               // i = 3
                if (!j || j == 5 || j == 8) // gray
                {
                    brick_marron.set_vertex(0, v1);
                    brick_marron.set_vertex(1, v2);
                    brick_marron.set_vertex(2, v3);
                    brick_marron.set_vertex(3, v4);
                    bricks.push_back(brick_marron);
                }
                else // purple
                {
                    brick_purple.set_vertex(0, v1);
                    brick_purple.set_vertex(1, v2);
                    brick_purple.set_vertex(2, v3);
                    brick_purple.set_vertex(3, v4);
                    bricks.push_back(brick_purple);
                }
            } // i = 3
            if (i == 4)
            {                                   // i = 4
                if (j == 1 || j == 2 || j == 3) // blue
                {
                    brick_blue.set_vertex(0, v1);
                    brick_blue.set_vertex(1, v2);
                    brick_blue.set_vertex(2, v3);
                    brick_blue.set_vertex(3, v4);
                    bricks.push_back(brick_blue);
                }
                else
                { // gray
                    brick_marron.set_vertex(0, v1);
                    brick_marron.set_vertex(1, v2);
                    brick_marron.set_vertex(2, v3);
                    brick_marron.set_vertex(3, v4);
                    bricks.push_back(brick_marron);
                }
            } // i = 4
            if (i == 5)
            {                                   // 5
                if (j == 1 || j == 2 || j == 3) // dark blue
                {
                    brick_dark_blue.set_vertex(0, v1);
                    brick_dark_blue.set_vertex(1, v2);
                    brick_dark_blue.set_vertex(2, v3);
                    brick_dark_blue.set_vertex(3, v4);
                    bricks.push_back(brick_dark_blue);
                }
                else
                { // gray
                    brick_marron.set_vertex(0, v1);
                    brick_marron.set_vertex(1, v2);
                    brick_marron.set_vertex(2, v3);
                    brick_marron.set_vertex(3, v4);
                    bricks.push_back(brick_marron);
                }

            } // 5

            if (i == 6)
            {                               // 6
                if (!j || j == 5 || j == 8) // gray
                {
                    brick_marron.set_vertex(0, v1);
                    brick_marron.set_vertex(1, v2);
                    brick_marron.set_vertex(2, v3);
                    brick_marron.set_vertex(3, v4);
                    bricks.push_back(brick_marron);
                }
                else // green
                {
                    brick_green.set_vertex(0, v1);
                    brick_green.set_vertex(1, v2);
                    brick_green.set_vertex(2, v3);
                    brick_green.set_vertex(3, v4);
                    bricks.push_back(brick_green);
                }
            } // 6
            if (i == 7)
            { // 7
                if (!j || j == 1 || j == 2 || j == 5 ||
                    j == 8) // gray
                {
                    brick_marron.set_vertex(0, v1);
                    brick_marron.set_vertex(1, v2);
                    brick_marron.set_vertex(2, v3);
                    brick_marron.set_vertex(3, v4);
                    bricks.push_back(brick_marron);
                }
                else // dark_green
                {
                    brick_dark_green.set_vertex(0, v1);
                    brick_dark_green.set_vertex(1, v2);
                    brick_dark_green.set_vertex(2, v3);
                    brick_dark_green.set_vertex(3, v4);
                    bricks.push_back(brick_dark_green);
                }
            } // 7
            if (i == 8)
            {                         // 8
                if (j == 1 || j == 2) // gray
                {
                    brick_marron.set_vertex(0, v1);
                    brick_marron.set_vertex(1, v2);
                    brick_marron.set_vertex(2, v3);
                    brick_marron.set_vertex(3, v4);
                    bricks.push_back(brick_marron);
                }
                else // orange
                {
                    brick_orange.set_vertex(0, v1);
                    brick_orange.set_vertex(1, v2);
                    brick_orange.set_vertex(2, v3);
                    brick_orange.set_vertex(3, v4);
                    bricks.push_back(brick_orange);
                }
            } // 8
        }
    }
}

void Arcanoid::init_ball()
{
    std::string ball_path = "resources/ball/ball.png";
    ball_texture = std::make_unique<Texture>(ball_path);

    ball = std::make_unique<Ball>(ball_path);
}

void Arcanoid::init_platform()
{
    std::string platform_path =
        "resources/platform/platform1.png";
    platform_texture =
        std::make_unique<Texture>(platform_path);

    platform = std::make_unique<Platform>(platform_path);

    float stride = 0.18f;
    platform->set_vertex(
        0,
        Vertex2{ { 0.f - platform->get_width() / 2.f,
                   -1.f + stride + platform->get_height() },
                 { 0.f, 1.f } });
    platform->set_vertex(
        1,
        Vertex2{ { 0.f - platform->get_width() / 2.f,
                   -1.f + stride },
                 { 0.f, 0.f } });
    platform->set_vertex(
        2,
        Vertex2{ { 0.f + platform->get_width() / 2.f,
                   -1.f + stride },
                 { 1.f, 0.f } });
    platform->set_vertex(
        3,
        Vertex2{ { 0.f + platform->get_width() / 2.f,
                   -1.f + stride + platform->get_height() },
                 { 1.f, 1.f } });
}

void Arcanoid::init_health()
{
    std::string health_path = "resources/life/life.png";
    health_texture = std::make_unique<Texture>(health_path);

    float stride = 0.03f;
    for (int i = 0; i < 3; ++i)
    {
        Life* life = new Life(health_path);

        life->set_vertex(
            0,
            Vertex2{
                { -1.f + i * stride + i * life->get_width(),
                  -1.f + life->get_height() },
                { 0.f, 1.f } });
        life->set_vertex(1,
                         Vertex2{ { -1.f + i * stride +
                                        i * life->get_width(),
                                    -1.f },
                                  { 0.f, 0.f } });
        life->set_vertex(
            2,
            Vertex2{ { -1.f + life->get_width() +
                           +i * life->get_width() + i * stride,
                       -1.f },
                     { 1.f, 0.f } });
        life->set_vertex(
            3,
            Vertex2{ { -1.f + life->get_width() + i * stride +
                           i * life->get_width(),
                       -1.f + life->get_height() },
                     { 1.f, 1.f } });
        health.push_back(life);
    }
}

size_t Arcanoid::game_init()
{
    using namespace eng;

    engine = create_engine();

    if (engine == nullptr)
    {
        std::cerr << "Error creating engine!" << std::endl;
        return 1;
    }

    if (engine->init())
    {
        std::cerr << "Error init engine" << std::endl;
        return 1;
    }

    //---------------background init------------------
    init_background();
    //---------------walls init-----------------------
    init_walls();
    //---------------bricks init-----------------------
    init_bricks();
    //---------------ball init-------------------------
    init_ball();
    //---------------platform init---------------------
    init_platform();
    //---------------health init-----------------------
    init_health();

    is_running = true;
    return 0;
}

void Arcanoid::game_event(float delta_time_frame)
{
    using namespace eng;

    Events     event;
    static int left  = 0;
    static int right = 0;
    while (engine->read_input(event))
    {

        if (event == Events::turn_off)
        {
            is_running = false;
            break;
        }

        if (event == Events::left_pressed)
        {
            platform_move(DirectionPlatform::left,
                          delta_time_frame);
            std::cout << "left = " << ++left << std::endl;
        }
        if (event == Events::right_pressed)
        {
            std::cout << "right = " << ++right << std::endl;
            platform_move(DirectionPlatform::right,
                          delta_time_frame);
        }
    }
}

void Arcanoid::game_update() {}

void Arcanoid::game_render()
{
    render_background();
    // render_walls();
    render_bricks();
    render_ball();
    render_health();
    render_platform();

    engine->swap_buffers();
}

void Arcanoid::render_background() const
{
    Vertex2 v1_text_tr1{ { -1.f, 1.f }, { 0.f, 1.f } };
    Vertex2 v2_text_tr1{ { -1.f, -1.f }, { 0.f, 0.f } };
    Vertex2 v3_text_tr1{ { 1.f, -1.f }, { 1.f, 0.f } };

    Triangle2 tr_text_low{ v1_text_tr1,
                           v2_text_tr1,
                           v3_text_tr1 };

    Vertex2 v1_text_tr2{ { -1.f, 1.f }, { 0.f, 1.f } };
    Vertex2 v2_text_tr2{ { 1.f, 1.f }, { 1.f, 1.f } };
    Vertex2 v3_text_tr2{ { 1.f, -1.f }, { 1.f, 0.f } };

    Triangle2 tr_text_high{ v1_text_tr2,
                            v2_text_tr2,
                            v3_text_tr2 };

    Matrix m = Matrix::scale(1.0);
    engine->render_triangle(tr_text_low, background.get(), m);
    engine->render_triangle(tr_text_high, background.get(), m);
}

void Arcanoid::render_walls()
{
    for (auto& el : walls)
    {
        Triangle1 tr_low{ el[0], el[1], el[2] };

        Triangle1 tr_high{ el[0], el[3], el[2] };

        engine->render_triangle(tr_low);
        engine->render_triangle(tr_high);
    }
}

void Arcanoid::render_bricks() const
{
    for (auto& el : bricks)
    {
        Triangle2 tr_text_low{ el.get_vertex(0),
                               el.get_vertex(1),
                               el.get_vertex(2) };

        Triangle2 tr_text_high{ el.get_vertex(0),
                                el.get_vertex(3),
                                el.get_vertex(2) };
        Matrix    m  = Matrix::scale(1.0);
        auto      it = std::find_if(
            v_bricks_textures.begin(),
            v_bricks_textures.end(),
            [&el](const Texture* const texture) {
                return texture->get_path() == el.get_path();
            });
        if (it != v_bricks_textures.end())
        {
            engine->render_triangle(tr_text_low, *it, m);
            engine->render_triangle(tr_text_high, *it, m);
        }
    }
}

void Arcanoid::render_ball() const
{
    Triangle2 tr_text_low{ ball->get_vertex(0),
                           ball->get_vertex(1),
                           ball->get_vertex(2) };

    Triangle2 tr_text_high{ ball->get_vertex(0),
                            ball->get_vertex(3),
                            ball->get_vertex(2) };
    Matrix    m = Matrix::scale(1.0);

    engine->render_triangle(
        tr_text_low, ball_texture.get(), m);
    engine->render_triangle(
        tr_text_high, ball_texture.get(), m);
}

void Arcanoid::render_health() const
{
    for (auto& el : health)
    {
        Triangle2 tr_text_low{ el->get_vertex(0),
                               el->get_vertex(1),
                               el->get_vertex(2) };

        Triangle2 tr_text_high{ el->get_vertex(0),
                                el->get_vertex(3),
                                el->get_vertex(2) };
        Matrix    m = Matrix::scale(1.0);

        engine->render_triangle(
            tr_text_low, health_texture.get(), m);
        engine->render_triangle(
            tr_text_high, health_texture.get(), m);
    }
}

void Arcanoid::render_platform() const
{
    Triangle2 tr_text_low{ platform->get_vertex(0),
                           platform->get_vertex(1),
                           platform->get_vertex(2) };

    Triangle2 tr_text_high{ platform->get_vertex(0),
                            platform->get_vertex(3),
                            platform->get_vertex(2) };

    //    engine->render_triangle(tr_text_low,
    //                            platform_texture.get(),
    //                            platform->get_matrix());
    //    engine->render_triangle(tr_text_high,
    //                            platform_texture.get(),
    //                            platform->get_matrix());
    engine->render_triangle(tr_text_low,
                            platform_texture.get());
    engine->render_triangle(tr_text_high,
                            platform_texture.get());
}

void Arcanoid::platform_move(DirectionPlatform dir,
                             float delta_time_frame)
{
    Matrix m;
    Vector move;

    if (dir == DirectionPlatform::left)
    {
        // Vector  left{ -0.05f * delta_time_frame, 0.f };
        move.x = -platform->get_speed() * delta_time_frame;
        move.y = 0.f;
        m      = Matrix::move(move);
        Vertex2 tmp1 = platform->get_vertex(0);
        Vertex2 res1 = tmp1 * m;

        // check platform in boundary window
        if (res1.v_pos.x <= -1.f)
            return;
    }

    if (dir == DirectionPlatform::right)
    {
        move.x = platform->get_speed() * delta_time_frame;
        move.y = 0.f;
        m      = Matrix::move(move);
        Vertex2 tmp2 = platform->get_vertex(2);
        Vertex2 res2 = tmp2 * m;

        if (res2.v_pos.x >= 1.f)
            return;
    }

    // move platform
    platform->set_vertex(0, platform->get_vertex(0) * m);
    platform->set_vertex(1, platform->get_vertex(1) * m);
    platform->set_vertex(2, platform->get_vertex(2) * m);
    platform->set_vertex(3, platform->get_vertex(3) * m);
}

void Arcanoid::game_uninit()
{
    engine->uninit();
    destroy_engine(engine);
}
