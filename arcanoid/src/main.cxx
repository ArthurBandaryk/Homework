#include <algorithm>
#include <cmath>
#include <iostream>

#include "game.hxx"
//#define DEBUG

int main(int /*argc*/, char* /*argv*/[])
{
    using namespace eng;

    //-----------------------------------------------------

    Arcanoid arcanoid;
    arcanoid.game_init();

    bool loop_continue = true;

    auto time_prev_frame =
        std::chrono::high_resolution_clock::now();
    decltype(std::chrono::high_resolution_clock::now())
        time_cur_frame;

    while (loop_continue)
    { // 1

        time_cur_frame =
            std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> delta_time_frame =
            time_cur_frame - time_prev_frame;
        time_prev_frame = time_cur_frame;

#ifdef DEBUG
        std::cout << "delta = " << delta_time_frame.count()
                  << std::endl;
#endif

        arcanoid.game_event();
        if (!arcanoid.game_running())
            break;
        arcanoid.game_update(delta_time_frame.count());
        if (!arcanoid.game_running())
            break;
        arcanoid.game_render();

    } // 1

    arcanoid.game_uninit();

    return EXIT_SUCCESS;
}
