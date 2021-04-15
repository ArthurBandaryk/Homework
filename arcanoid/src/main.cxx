#include <algorithm>
#include <cmath>
#include <iostream>

#include "game.hxx"

int main(int /*argc*/, char* /*argv*/[])
{
    using namespace eng;

    //-----------------------------------------------------

    Arcanoid arcanoid;
    // float width_brick  = 100.f * 2.f / 1024.f;
    // float height_brick = 20.f * 2.f / 768.f;
    arcanoid.game_init();

    bool loop_continue = true;

    while (loop_continue)
    { // 1

        arcanoid.game_event();
        if (!arcanoid.game_running())
            break;
        arcanoid.game_update();
        arcanoid.game_render();

    } // 1

    arcanoid.game_uninit();

    return EXIT_SUCCESS;
}
