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

    bool loop_continue = true;
    while (loop_continue)
    { // 1
        Events event;
        while (engine->read_input(event))
        {
            switch (event)
            {
                case Events::turn_off:
                    loop_continue = false;
                    break;
                case Events::down_pressed:
                    // std::cout << "down pressed" << std::endl;
                    break;
                case Events::down_released:
                    // std::cout << "down released" << std::endl;
                    break;
                case Events::up_pressed:
                    // std::cout << "up pressed" << std::endl;
                    break;
                case Events::up_released:
                    // std::cout << "up released" << std::endl;
                    break;
                case Events::mouse_left_pressed:
                    // std::cout << "mouse left pressed" << std::endl;
                    break;
                case Events::mouse_left_released:
                    // std::cout << "mouse left released" << std::endl;
                    break;
                case Events::mouse_right_pressed:
                    // std::cout << "mouse right pressed" << std::endl;
                    break;
                case Events::mouse_right_released:
                    // std::cout << "mouse right released" << std::endl;
                    break;
            }
        }
        if (!engine->update())
            loop_continue = false;
    } // 1

    engine->uninit();
    destroy_engine(engine);

    return EXIT_SUCCESS;
}
