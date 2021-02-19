#include "my_gfx_program.hxx"

#include <SDL2/SDL.h>

int main(int /*argc*/, char* /*argv*/[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        std::cerr << "Error on init sdl libs: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    constexpr size_t window_width  = 320;
    constexpr size_t window_height = 240;
    SDL_Window*      window        = SDL_CreateWindow("First shader",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          window_width,
                                          window_height,
                                          SDL_WINDOW_OPENGL);

    if (window == nullptr)
    {
        std::cerr << "Error on creating window!" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        std::cerr << "Error creating render" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    bool loop_continue = true;

    canv::Image texture_img;
    texture_img.load_image("1.ppm");

    canv::Image img{ texture_img.get_width(), texture_img.get_height() };
    canv::Color black = { 0, 0, 0 };

    // vertexes for loaded image
    std::vector<Vertex> v_vertexes1{
        { 0., 0., 0., 0., 0, 0, 0, 0 },
        { static_cast<double>(img.get_width() - 1),
          static_cast<double>(img.get_height() - 1),
          1.,
          1.,
          0,
          0,
          0,
          0 },
        { 0., static_cast<double>(img.get_height() - 1), 0., 1., 0, 0, 0, 0 },
        { static_cast<double>(img.get_width() - 1), 0., 1., 0., 0, 0, 0, 0 }
    };

    // indexes for loaded image
    std::vector<short> v_indexes1{ { 0, 1, 2, 0, 3, 1 } };

    constexpr size_t size_cube = 30;
    // vertexes for cube

    // clang-format off


    //               [1]----------------[2]
    //               /|                 /|
    //              / |                / |
    //             /  |               /  |
    //            /   |              /   |
    //           /   [5]------------/---[6]
    //          /    /             /    /
    //         [0]----------------[3]  /
    //          |  /               |  /
    //          | /                | /
    //          |/                 |/
    //         [4]----------------[7]

    std::vector<Vertex> v_vertexes2{
        {   //0 vertex
            static_cast<double>(window_width / 2 - size_cube),
            static_cast<double>(window_height / 2 - size_cube),
            -1.,
            1.,
            0,
            0,
            255,
            0
        },

        {   //1 vertex
            static_cast<double>(window_width / 2 - (size_cube / 4)),
            static_cast<double>(window_height / 2 - (2*size_cube)),
            -1.,
            1.,
            0,
            0,
            255,
            0
        },

        {   //2 vertex
            static_cast<double>(window_width / 2 + (7*size_cube/4)),
            static_cast<double>(window_height / 2 - (2*size_cube)),
            1.,
            1.,
            0,
            0,
            255,
            0
        },

        {   //3 vertex
            static_cast<double>(window_width / 2 + size_cube),
            static_cast<double>(window_height / 2 - size_cube),
            1.,
            1.,
            0,
            0,
            255,
            0
        },

        {
            //4 vertex
            static_cast<double>(window_width / 2 - size_cube),
            static_cast<double>(window_height / 2 + size_cube),
            -1.,
            -1.,
            0,
            0,
            255,
            0
        },

        {//5 vertex
            static_cast<double>(window_width / 2 - (size_cube / 4)),
            static_cast<double>(window_height / 2),
            -1.,
            -1.,
            0,
            0,
            255,
            0
        },


        {//6 vertex
            static_cast<double>(window_width / 2 + (7*size_cube/4)),
            static_cast<double>(window_height / 2),
            1.,
            -1.,
            0,
            0,
            255,
            0
        },

        {//7 vertex
            static_cast<double>(window_width / 2 + size_cube),
            static_cast<double>(window_height / 2 + size_cube),
            1.,
            -1.,
            0,
            0,
            255,
            0  }
    };



    // indexes for cube
    std::vector<short> v_indexes2
    {
        0, 1, 2,     2, 3, 0,
        4, 5, 6,     6, 7, 4,
        0, 1, 5,     0, 5, 4,
        3, 2, 2,     3, 6, 7,
        0, 4, 7,     0, 3, 7,
        1, 5, 6,     6, 2, 1
    };

    //for lines of cube
    std::vector<short> v_indexes3
    {
      0, 1, 1, 2, 2, 3, 3, 0,
      4, 5, 5, 6, 6, 7, 7, 4,
      0, 4, 4, 5, 5, 1, 1, 0,
      3, 7, 7, 6, 6, 2, 2, 3,
      0, 4, 4, 7, 7, 3 ,3, 0,
      5, 6, 6, 2, 2, 1, 1, 5
    };

    // clang-format on

    TriangleInterpolatedRender render_triangle(img);

    simple_gfx_prog             prog1;
    gray_transform_gfx          prog2;
    cube_gfx_prog               prog3;
    std::array<gfx_program*, 3> array_gfx{ &prog1, &prog2, &prog3 };
    int                         index_prog = 0;

    render_triangle.set_gfx_program(*array_gfx.at(index_prog));

    while (loop_continue)
    { // 1
        SDL_Event event;
        while (SDL_PollEvent(&event))
        { // 2
            switch (event.type)
            {
                case SDL_QUIT:
                    loop_continue = false;
                    break;
                case SDL_MOUSEWHEEL:
                    if (event.wheel.y > 0) // scroll up
                    {
                        ++index_prog;
                        if (index_prog >= static_cast<int>(array_gfx.size()))
                            index_prog = 0;
                    }
                    if (event.wheel.y < 0) // scroll down
                    {
                        --index_prog;
                        if (index_prog < 0)
                            index_prog = array_gfx.size() - 1;
                    }
                    render_triangle.set_gfx_program(*array_gfx.at(index_prog));
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_SPACE)
                    {
                        index_prog = (index_prog + 1) % array_gfx.size();
                        render_triangle.set_gfx_program(
                            *array_gfx.at(index_prog));
                    }
                    break;
            }
        } // 2

        std::vector<Vertex> v_vertexes;
        std::vector<short>  v_indexes;

        if (index_prog < 2)
        {
            v_vertexes = v_vertexes1;
            v_indexes  = v_indexes1;
        }
        else
        {
            v_vertexes = v_vertexes2;
            v_indexes  = v_indexes3;
        }

        Uniform uni{ 0., 0., 0., 0., static_cast<double>(SDL_GetTicks() / 1000),
                     0,  0,  0,  0,  &texture_img };
        render_triangle.clear(black);
        (*array_gfx.at(index_prog)).set_uniform(uni);
        if (index_prog < 2)
            render_triangle.draw_triangles_interpolated(v_vertexes, v_indexes);
        else
            render_triangle.draw_raster_line(v_vertexes, v_indexes);

        SDL_Surface* surface =
            SDL_CreateRGBSurfaceFrom(img.get_pixels().data(),
                                     img.get_width(),
                                     img.get_height(),
                                     sizeof(canv::Color) * 8,
                                     img.get_width() * sizeof(canv::Color),
                                     255,
                                     255,
                                     255,
                                     0);
        if (surface == nullptr)
        {
            std::cerr << "Error creating surface!!!" << std::endl;
            std::cerr << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return EXIT_FAILURE;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == nullptr)
        {
            std::cerr << "Error creating texture" << std::endl;
            std::cerr << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return EXIT_FAILURE;
        }
        SDL_FreeSurface(surface);

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);

        SDL_DestroyTexture(texture);
    } // 1

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
