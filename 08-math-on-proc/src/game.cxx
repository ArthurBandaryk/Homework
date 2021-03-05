#include <algorithm>
#include <cmath>
#include <iostream>

#include "my_engine.hxx"

int main(int /*argc*/, char* /*argv*/[])
{
    using namespace eng;
    IEngine* engine = nullptr;
    engine          = create_engine();
    if (engine == nullptr)
    {
        std::cerr << "Error creating engine!" << std::endl;
        return EXIT_FAILURE;
    }

    if (engine->init())
    {
        std::cerr << "Error init engine" << std::endl;
        return EXIT_FAILURE;
    }

    //-----------------------------------------------------

    Texture* tank_texture = engine->create_texture("tank.png");

    Vertex2 v1_text_tr1{ { -0.3f, 0.3f }, { 0.f, 1.f - 1.f } };
    Vertex2 v2_text_tr1{ { -0.3f, -0.3f },
                         { 0.f, 1.f - 0.f } };
    Vertex2 v3_text_tr1{ { 0.3f, -0.3f }, { 1.f, 1.f - 0.f } };

    Triangle2 tr_text_low{ v1_text_tr1,
                           v2_text_tr1,
                           v3_text_tr1 };

    Vertex2 v1_text_tr2{ { -0.3f, 0.3f }, { 0.f, 1.f - 1.f } };
    Vertex2 v2_text_tr2{ { 0.3f, 0.3f }, { 1.f, 1.f - 1.f } };
    Vertex2 v3_text_tr2{ { 0.3f, -0.3f }, { 1.f, 1.f - 0.f } };

    Triangle2 tr_text_high{ v1_text_tr2,
                            v2_text_tr2,
                            v3_text_tr2 };

    Matrix rot_up   = Matrix::rotate(5.f);
    Matrix rot_down = Matrix::rotate(-5.f);

    Vector v_left{ -0.1f, 0.f };
    Vector v_right{ 0.1f, 0.f };
    Vector v_up{ 0.f, 0.1f };
    Vector v_down{ 0.f, -0.1f };
    Matrix move_left  = Matrix::move(v_left);
    Matrix move_right = Matrix::move(v_right);
    Matrix move_up    = Matrix::move(v_up);
    Matrix move_down  = Matrix::move(v_down);

    Matrix scale_reduce  = Matrix::scale(-0.1f);
    Matrix scale_enlarge = Matrix::scale(0.1f);
    Matrix aspect;
    aspect.col1.x = 1.f;
    aspect.col1.y = 0.f;
    aspect.col2.x = 0.f;
    aspect.col2.y = 800.f / 600.f;
    aspect.col3.x = 0.f;
    aspect.col3.y = 0.f;

    for (auto& el : tr_text_low.triangle)
    {
        el.v_pos = el.v_pos * aspect;
    }

    for (auto& el : tr_text_high.triangle)
    {
        el.v_pos = el.v_pos * aspect;
    }

    bool loop_continue = true;

    while (loop_continue)
    { // 1

        Events event;
        while (engine->read_input(event))
        {
            if (event == Events::turn_off)
            {
                loop_continue = false;
                break;
            }
            if (event == Events::up_pressed)
            {
                for (auto& el : tr_text_high.triangle)
                {
                    el.v_pos = el.v_pos * move_up;
                }

                for (auto& el : tr_text_low.triangle)
                {
                    el.v_pos = el.v_pos * move_up;
                }
            }

            if (event == Events::down_pressed)
            {
                for (auto& el : tr_text_high.triangle)
                {
                    el.v_pos = el.v_pos * move_down;
                }

                for (auto& el : tr_text_low.triangle)
                {
                    el.v_pos = el.v_pos * move_down;
                }
            }

            if (event == Events::left_pressed)
            {
                for (auto& el : tr_text_high.triangle)
                {
                    el.v_pos = el.v_pos * move_left;
                }

                for (auto& el : tr_text_low.triangle)
                {
                    el.v_pos = el.v_pos * move_left;
                }
            }
            if (event == Events::right_pressed)
            {
                for (auto& el : tr_text_high.triangle)
                {
                    el.v_pos = el.v_pos * move_right;
                }

                for (auto& el : tr_text_low.triangle)
                {
                    el.v_pos = el.v_pos * move_right;
                }
            }

            if (event == Events::button1_X_pressed)
            {
                Vector centre_texture{
                    (tr_text_low.triangle[0].v_pos.x +
                     tr_text_low.triangle[2].v_pos.x) /
                        2.f,
                    (tr_text_low.triangle[0].v_pos.y +
                     tr_text_low.triangle[2].v_pos.y) /
                        2.f
                };
                Matrix move_t = Matrix::move(centre_texture);
                centre_texture.x *= -1.f;
                centre_texture.y *= -1.f;
                Matrix move_t_b = Matrix::move(centre_texture);
                Matrix r        = move_t * rot_up * move_t_b;
                for (auto& el : tr_text_low.triangle)
                {
                    el.v_pos = el.v_pos * r;
                }
                for (auto& el : tr_text_high.triangle)
                {
                    el.v_pos = el.v_pos * r;
                }
            }

            if (event == Events::button2_A_pressed)
            {
                Vector centre_texture{
                    (tr_text_low.triangle[0].v_pos.x +
                     tr_text_low.triangle[2].v_pos.x) /
                        2.f,
                    (tr_text_low.triangle[0].v_pos.y +
                     tr_text_low.triangle[2].v_pos.y) /
                        2.f
                };
                Matrix move_t = Matrix::move(centre_texture);
                centre_texture.x *= -1.f;
                centre_texture.y *= -1.f;
                Matrix move_t_b = Matrix::move(centre_texture);
                Matrix r        = move_t * rot_down * move_t_b;
                for (auto& el : tr_text_low.triangle)
                {
                    el.v_pos = el.v_pos * r;
                }
                for (auto& el : tr_text_high.triangle)
                {
                    el.v_pos = el.v_pos * r;
                }
            }
        }

        engine->render_triangle(tr_text_low, tank_texture);
        engine->render_triangle(tr_text_high, tank_texture);

        engine->swap_buffers();

    } // 1

    engine->destroy_texture(tank_texture);
    engine->uninit();
    destroy_engine(engine);

    return EXIT_SUCCESS;
}
