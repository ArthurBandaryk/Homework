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

    Vertex v1_tr1{ -0.5f, -0.5f, 0.f, 0.f };
    Vertex v2_tr1{ 0.5f, -0.5f, 0.f, 0.f };
    Vertex v3_tr1{ -0.5f, 0.5f, 0.f, 0.f };

    Triangle tr1_q{ v1_tr1, v2_tr1, v3_tr1 };

    //-------------------------------------------------

    Vertex v1_tr2{ -0.5f, 0.5f, 0.f, 0.f };
    Vertex v2_tr2{ 0.5f, 0.5f, 0.f, 0.f };
    Vertex v3_tr2{ 0.5f, -0.5f, 0.f, 0.f };

    Triangle tr2_q{ v1_tr2, v2_tr2, v3_tr2 };

    //------------------------------------------------

    Vertex v1_tr3{ -0.5f, -0.5f, 0.f, 0.f };
    Vertex v2_tr3{ 0.5f, -0.5f, 0.f, 0.f };
    Vertex v3_tr3{ 0.f, 0.5f, 0.f, 0.f };

    Triangle tr_aim_1{ v1_tr3, v2_tr3, v3_tr3 };

    //------------------------------------------------

    Vertex v1_tr4{ 0.f, 0.5f, 0.f, 0.f };
    Vertex v2_tr4{ 0.f, 0.5f, 0.f, 0.f };
    Vertex v3_tr4{ 0.5f, -0.5f, 0.f, 0.f };

    Triangle tr_aim_2{ v1_tr4, v2_tr4, v3_tr4 };

    //-----------------------------------------------------

    Vertex v1_text_tr1{ -0.5f, 0.5f, 0.f, 1.f - 1.f };
    Vertex v2_text_tr1{ -0.5f, -0.5f, 0.f, 1.f - 0.f };
    Vertex v3_text_tr1{ 0.5f, -0.5f, 1.f, 1.f - 0.f };

    Triangle tr_text_low{ v1_text_tr1,
                          v2_text_tr1,
                          v3_text_tr1 };

    Vertex v1_text_tr2{ -0.5f, 0.5f, 0.f, 1.f - 1.f };
    Vertex v2_text_tr2{ 0.5f, 0.5f, 1.f, 1.f - 1.f };
    Vertex v3_text_tr2{ 0.5f, -0.5f, 1.f, 1.f - 0.f };

    Triangle tr_text_high{ v1_text_tr2,
                           v2_text_tr2,
                           v3_text_tr2 };

    bool loop_continue = true;

    while (loop_continue)
    { // 1
        Events event;
        while (engine->read_input(event))
        {
            if (event == Events::turn_off)
                loop_continue = false;
        }

        float delta = engine->get_time_from_init();
        float t     = std::sin(delta) * 0.5f + 0.5f;

        Triangle low  = interpolate(tr1_q, tr_aim_1, t);
        Triangle high = interpolate(tr2_q, tr_aim_2, t);
        // engine->render_triangle(low);
        // engine->render_triangle(high);
        engine->render_triangle(tr_text_low);
        engine->render_triangle(tr_text_high);

        engine->swap_buffers();

    } // 1

    engine->uninit();
    destroy_engine(engine);

    return EXIT_SUCCESS;
}
