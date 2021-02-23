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

    Triangle tr{};
    bool     loop_continue = true;

    while (loop_continue)
    { // 1
        Events event;
        while (engine->read_input(event))
        {
            if (event == Events::turn_off)
                loop_continue = false;
        }

        engine->render_triangle(tr);
        engine->swap_buffers();

    } // 1

    engine->uninit();
    destroy_engine(engine);

    return EXIT_SUCCESS;
}
