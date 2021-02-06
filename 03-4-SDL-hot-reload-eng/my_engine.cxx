#include <SDL.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <string_view>
#include <thread>
#include <vector>

#include "my_engine.hxx"

namespace eng
{

static bool         engine_exists = false;
static unsigned int num_rects     = 0;

static std::chrono::time_point<std::chrono::high_resolution_clock> time_cur;
static std::chrono::time_point<std::chrono::high_resolution_clock> time_prev;

constexpr int          WIDTH         = 1024;
constexpr int          HEIGHT        = 768;
constexpr int          AIM_WIDTH     = 30;
constexpr int          AIM_HEIGHT    = 30;
constexpr unsigned int MAX_AIMS      = 40;
constexpr float        TIME_INTERVAL = 0.5f;

SDL_Window*   window = nullptr;
SDL_Renderer* render = nullptr;

std::vector<SDL_Rect> v_rects; // aims

enum class DevBinds
{
    keypad,
    mouse
};

struct Keys
{
    DevBinds         type;
    SDL_KeyCode      code_board;
    Uint8            code_mouse;
    std::string_view name;
    Events           key_pressed;
    Events           key_released;
    Keys() {}
    Keys(DevBinds         type,
         SDL_KeyCode      code_board,
         Uint8            code_mouse,
         std::string_view name,
         Events           kp,
         Events           kr)
    {
        this->type         = type;
        this->code_board   = code_board;
        this->code_mouse   = code_mouse;
        this->name         = name;
        this->key_pressed  = kp;
        this->key_released = kr;
    }
};

std::array<Keys, 6> keys_binds{ Keys{ DevBinds::keypad,
                                      SDLK_w,
                                      Uint8(),
                                      "up",
                                      Events::up_pressed,
                                      Events::up_released },
                                Keys{ DevBinds::keypad,
                                      SDLK_UP,
                                      Uint8(),
                                      "up",
                                      Events::up_pressed,
                                      Events::up_released },
                                Keys{ DevBinds::keypad,
                                      SDLK_s,
                                      Uint8(),
                                      "down",
                                      Events::down_pressed,
                                      Events::down_released },
                                Keys{ DevBinds::keypad,
                                      SDLK_DOWN,
                                      Uint8(),
                                      "down",
                                      Events::down_pressed,
                                      Events::down_released },
                                Keys{ DevBinds::mouse,
                                      SDL_KeyCode(),
                                      SDL_BUTTON_LEFT,
                                      "left mouse's button",
                                      Events::mouse_left_pressed,
                                      Events::mouse_left_released },
                                Keys{ DevBinds::mouse,
                                      SDL_KeyCode(),
                                      SDL_BUTTON_RIGHT,
                                      "right mouse's button",
                                      Events::mouse_right_pressed,
                                      Events::mouse_right_released } };

IEngine::~IEngine() {}

IEngine* create_engine()
{
    if (engine_exists)
        return nullptr;
    IEngine* engine = new Engine_impl();
    engine_exists   = true;
    return engine;
}

void destroy_engine(IEngine*& engine)
{
    if (!engine_exists || engine == nullptr)
        return;
    engine_exists = false;
    delete engine;
    engine = nullptr;
}

bool shoot(SDL_Event* ev)
{
    auto res = std::find_if(
        v_rects.begin(), v_rects.end(), [&ev](const SDL_Rect& rect) -> bool {
            if (ev->button.x >= rect.x && ev->button.x <= rect.x + AIM_WIDTH)
            {
                if (ev->button.y >= rect.y &&
                    ev->button.y <= rect.y + AIM_HEIGHT)
                    return true;
            }
            return false;
        });

    if (res != v_rects.end())
    {
        v_rects.erase(res);
        num_rects--;
        return true;
    }
    else
        return false;
}

bool check_input(const SDL_Event* const sdl_ev, Keys& k, DevBinds db)
{
    auto res = keys_binds.end();
    if (db == DevBinds::keypad)
    { // 1
        res = std::find_if(keys_binds.begin(),
                           keys_binds.end(),
                           [&sdl_ev](const Keys& ks) -> bool {
                               return sdl_ev->key.keysym.sym == ks.code_board;
                           });
        if (res == keys_binds.end())
            return false;
        else
        {
            k = *res;
            return true;
        }
    } // 1
    else
    { // 2
        res = std::find_if(keys_binds.begin(),
                           keys_binds.end(),
                           [&sdl_ev](const Keys& ks) -> bool {
                               return sdl_ev->button.button == ks.code_mouse;
                           });
        if (res == keys_binds.end())
            return false;
        else
        {
            k = *res;
            return true;
        }
    } // 2
}

int Engine_impl::init()
{
    int res = SDL_Init(SDL_INIT_EVERYTHING);
    if (res)
    {
        std::cerr << "Error initialization SDL libs! \n"
                  << SDL_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("Aim training",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WIDTH,
                              HEIGHT,
                              SDL_WINDOW_OPENGL);
    if (window == nullptr)
    {
        std::cerr << "Error creating window!\n";
        std::cerr << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (render == nullptr)
    {
        std::cerr << "Error creating render!" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        return 1;
    }

    v_rects.reserve(10);

    time_prev = std::chrono::high_resolution_clock::now();
    time_cur  = std::chrono::high_resolution_clock::now();

    srand(time(NULL));

    return 0;
} // end init()

void Engine_impl::uninit()
{
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Engine_impl::read_input(Events& ev)
{
    SDL_Event event;
    if (!SDL_PollEvent(&event))
        return false;
    bool exit_success = false;
    Keys ks;
    switch (event.type)
    {
        case SDL_QUIT:
            ev           = Events::turn_off;
            exit_success = true;
            break;

        case SDL_KEYDOWN:
            exit_success = check_input(&event, ks, DevBinds::keypad);
            ev           = ks.key_pressed;
            break;

        case SDL_KEYUP:
            exit_success = check_input(&event, ks, DevBinds::keypad);
            ev           = ks.key_released;
            SDL_SetRenderDrawColor(render, 235, 230, 87, 255);
            break;

        case SDL_MOUSEBUTTONDOWN:
            exit_success = check_input(&event, ks, DevBinds::mouse);
            ev           = ks.key_pressed;
            shoot(&event);
            break;

        case SDL_MOUSEBUTTONUP:
            exit_success = check_input(&event, ks, DevBinds::mouse);
            ev           = ks.key_released;
            break;
    }
    return exit_success;
}

bool Engine_impl::update()
{
    time_cur = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> delta_time = time_cur - time_prev;
    if (delta_time.count() > TIME_INTERVAL)
    {
        if (num_rects >= MAX_AIMS)
        {
            v_rects.clear();
            num_rects = 0;
            return false;
        }
        time_prev = time_cur;
        SDL_Rect rect;
        rect.x = rand() % WIDTH;
        rect.y = rand() % HEIGHT;
        if (rect.x + AIM_WIDTH > WIDTH)
            rect.x -= WIDTH;
        if (rect.y + AIM_HEIGHT > HEIGHT)
            rect.y -= AIM_HEIGHT;
        if (rect.x < 0)
            rect.x += AIM_WIDTH;
        if (rect.y < 0)
            rect.y += AIM_HEIGHT;
        rect.w = AIM_WIDTH;
        rect.h = AIM_HEIGHT;

        v_rects.emplace_back(rect);
        num_rects++;
    }
    //
    // SDL_UpdateWindowSurface(window);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderClear(render);
    for (const auto& el : v_rects)
    {
        SDL_SetRenderDrawColor(render, 235, 230, 87, 255);
        SDL_RenderFillRect(render, &el);
    }
    SDL_RenderPresent(render);

    return true;
}

} // namespace eng
eng::Game* reload_game(eng::Game*    old,
                       const char*   library_name,
                       const char*   tmp_library_name,
                       eng::IEngine& engine,
                       void*&        old_handle);

#ifdef __cplusplus
extern "C"
#endif

    int
    main(int /*argc*/, char* /*argv*/[])
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

    using namespace std::string_literals;
    const char* lib_name =
        SDL_GetPlatform() == "Windows"s ? "libgame.dll" : "./libgame.so";

    // const char* lib_name = "./libgame.so";
    const char* tmp_lib = "./temp.so";

    void* game_lib_ptr{};

    /*Game**/ Game* game =
        reload_game(nullptr, lib_name, tmp_lib, *engine, game_lib_ptr);

    auto time_during_loading = std::filesystem::last_write_time(lib_name);

    // game->initialize();

    bool loop_continue = true;
    while (loop_continue)
    { // 1

        auto current_write_time = std::filesystem::last_write_time(lib_name);

        if (current_write_time != time_during_loading)
        {
            std::filesystem::file_time_type next_write_time;
            // wait while library file fishish to changing
            for (;;)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                next_write_time = std::filesystem::last_write_time(lib_name);
                if (next_write_time != current_write_time)
                {
                    current_write_time = next_write_time;
                }
                else
                {
                    break;
                }
            };

            std::cout << "reloading game" << std::endl;
            game = reload_game(game, lib_name, tmp_lib, *engine, game_lib_ptr);

            if (game == nullptr)
            {
                std::cerr << "next attemp to reload game..." << std::endl;
                continue;
            }

            time_during_loading = next_write_time;
        }

        //-------------------------------------------------------------
        Events event;
        while (engine->read_input(event))
        {
            if (!game->on_event(event))
                loop_continue = false;
        }
        if (!engine->update())
            loop_continue = false;
        // if (game->game_over())
        //  loop_continue = false;
        // game->update();
        // game->render();
    } // 1

    engine->uninit();
    destroy_engine(engine);

    return EXIT_SUCCESS;
}

eng::Game* reload_game(eng::Game*    old,
                       const char*   library_name,
                       const char*   tmp_library_name,
                       eng::IEngine& engine,
                       void*&        old_handle)
{
    using namespace std::filesystem;

    if (old)
    {
        SDL_UnloadObject(old_handle);
    }

    if (std::filesystem::exists(tmp_library_name))
    {
        if (0 != std::filesystem::remove(tmp_library_name))
        {
            std::cerr << "error: can't remove: " << tmp_library_name
                      << std::endl;
            return nullptr;
        }
    }

    try
    {
        std::filesystem::copy(library_name, tmp_library_name); // throw on error
    }
    catch (const std::exception& ex)
    {
        std::cerr << "error: can't copy [" << library_name << "] to ["
                  << tmp_library_name << "]" << std::endl;
        return nullptr;
    }

    void* game_handle = SDL_LoadObject(tmp_library_name);

    if (game_handle == nullptr)
    {
        std::cerr << " error: failed to load: [" << tmp_library_name << "] "
                  << SDL_GetError() << std::endl;
        return nullptr;
    }

    old_handle = game_handle;

    void* create_game_func_ptr = SDL_LoadFunction(game_handle, "create_game");

    if (create_game_func_ptr == nullptr)
    {
        std::cerr << "error: no function [create_game] in " << tmp_library_name
                  << " " << SDL_GetError() << std::endl;
        return nullptr;
    }
    // void* destroy_game_func_ptr = SDL_LoadFunction(game_handle,
    // "destroy_game");

    typedef decltype(&create_game) create_game_ptr;

    auto create_game_func =
        reinterpret_cast<create_game_ptr>(create_game_func_ptr);

    eng::Game* game = create_game_func(&engine);

    if (game == nullptr)
    {
        std::cerr << "error: func [create_game] returned: nullptr" << std::endl;
        return nullptr;
    }
    return game;
}
