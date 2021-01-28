#include <iostream>
#include <cstdlib>

#include <SDL_version.h>
#include "Configure.h"

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cout << "Version " << argv[0]	<< ": " << FirstSDLProj_VERSION_MAJOR
                  << "." << FirstSDLProj_VERSION_MINOR << "."
                  << FirstSDLProj_VERSION_PATCH << std::endl;
    }

    SDL_version sdl_compiled { 0, 0, 0 };
    SDL_version sdl_linked { 0, 0, 0 };

    SDL_VERSION(&sdl_compiled)
    SDL_GetVersion(&sdl_linked);

    std::cout << "Linked with dynamic lib!!!" << std::endl;

    std::cout << "Compiled version: "
              << static_cast<int>(sdl_compiled.major) << "."
              << static_cast<int>(sdl_compiled.minor) << "."
              << static_cast<int>(sdl_compiled.patch) << std::endl;
    std::cout << "Linked version of libs: "
              << static_cast<int>(sdl_linked.major) << "."
              << static_cast<int>(sdl_linked.minor) << "."
              << static_cast<int>(sdl_linked.patch) << std::endl;
    //SDL_fmod(34.5, 56.5);

    return  std::cout.good() ? EXIT_SUCCESS : EXIT_FAILURE;
}

