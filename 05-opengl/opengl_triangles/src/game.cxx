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

    Vertex   v1{ 0.f, 1.f, 0.f };
    Vertex   v2{ -1.f, -1.f, 0.f };
    Vertex   v3{ 1.f, -1.f, 0.f };
    Triangle tr1{ v1, v2, v3 };

    Vertex   v4{ -0.75f, 0.f, 0.5f };
    Vertex   v5{ 0.75f, 0.f, 0.5f };
    Vertex   v6{ 0.f, -1.f, -0.5f };
    Triangle tr2{ v4, v5, v6 };

    bool loop_continue = true;

    while (loop_continue)
    { // 1
        Events event;
        while (engine->read_input(event))
        {
            if (event == Events::turn_off)
                loop_continue = false;
        }

        engine->render_triangle(tr1);
        engine->render_triangle(tr2);
        engine->swap_buffers();

    } // 1

    engine->uninit();
    destroy_engine(engine);

    return EXIT_SUCCESS;
}
