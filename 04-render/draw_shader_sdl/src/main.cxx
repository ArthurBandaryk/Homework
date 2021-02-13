#include "my_gfx_program.hxx"

#include <SDL2/SDL.h>

int main(int /*argc*/, char* /*argv*/[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        std::cerr << "Error on init sdl libs: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    SDL_Window* window = SDL_CreateWindow("First shader",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          canv::width,
                                          canv::height,
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

    double m_x, m_y, r;
    m_x = m_y = 0.;
    r         = 80.; // radius

    std::vector<Vertex> v_vertexes;
    std::vector<short>  v_indexes;

    constexpr size_t num_cells_width  = 20;
    constexpr size_t num_cells_height = 20;
    constexpr double cell_width =
        static_cast<double>((canv::width - 1) / num_cells_width);
    constexpr double cell_height =
        static_cast<double>((canv::height - 1) / num_cells_height);

    // fill v_vertexes
    for (size_t i = 0; i <= num_cells_height; ++i)
    {
        for (size_t j = 0; j <= num_cells_width; ++j)
        {
            double x, y;
            x = static_cast<double>(j * cell_width);
            y = static_cast<double>(i * cell_height);
            v_vertexes.push_back({ x, y, 0, 255, 0, 0 });
        }
    }

    // fill v_indexes
    for (size_t i = 0; i < num_cells_width; ++i)
    {
        for (size_t j = 0; j < num_cells_height; ++j)
        {
            short v0 = i * (num_cells_height + 1) + j;
            short v1 = v0 + num_cells_width + 1;
            short v2 = v1 + 1;
            short v3 = v0 + 1;

            v_indexes.insert(v_indexes.end(), { v0, v1, v0 });
            v_indexes.insert(v_indexes.end(), { v1, v2, v1 });
            v_indexes.insert(v_indexes.end(), { v2, v3, v2 });
            v_indexes.insert(v_indexes.end(), { v3, v0, v3 });
        }
    }

    canv::Color black = { 0, 0, 0 };

    canv::Image img;

    TriangleInterpolatedRender render_triangle(img);
    my_gfx_program             prog;
    render_triangle.set_gfx_program(prog);

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
                case SDL_MOUSEMOTION:
                    m_x = static_cast<double>(event.motion.x);
                    m_y = static_cast<double>(event.motion.y);
                    break;
            }
        } // 2

        Uniform uni{ m_x, m_y, 0, 0, 0, static_cast<uint8_t>(r) };
        render_triangle.clear(black);
        prog.set_uniform(uni);
        render_triangle.draw_triangles_interpolated(v_vertexes, v_indexes);

        SDL_Surface* surface =
            SDL_CreateRGBSurfaceFrom(img.get_pixels().data(),
                                     canv::width,
                                     canv::height,
                                     sizeof(canv::Color) * 8,
                                     canv::width * sizeof(canv::Color),
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
