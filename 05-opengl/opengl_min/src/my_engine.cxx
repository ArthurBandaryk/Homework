#include <SDL.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <string_view>
#include <vector>

#include "glad/glad.h"
#include "my_engine.hxx"

namespace eng
{

static bool engine_exists = false;

constexpr GLuint window_width  = 800;
constexpr GLuint window_height = 600;

SDL_Window* window = nullptr;

decltype(std::chrono::high_resolution_clock::now()) time_begin;

//------------------------opengl check error---------------
//---------------------------------------------------------
static void APIENTRY
callback_opengl_debug(GLenum                       source,
                      GLenum                       type,
                      GLuint                       id,
                      GLenum                       severity,
                      GLsizei                      length,
                      const GLchar*                message,
                      [[maybe_unused]] const void* userParam);

// clang-format off
void my_gl_check()
{
    size_t error = glGetError();

    if (error != GL_NO_ERROR)
    {
        switch (error)
        {
            case GL_INVALID_ENUM:
                std::cerr << "incorrect Glenum" << std::endl;
                break;
            case GL_INVALID_VALUE:
                std::cerr << "GL_INVALID_VALUE error:"
                             " a numeric argument is "
                             "out of range "
                          << std::endl;
            case GL_INVALID_OPERATION:
                std::cerr
                    << "GL_INVALID_OPERATION: "
                       "the specified operation is not "
                       "allowed in the current state"
                    << std::endl;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                std::cerr
                    << "GL_INVALID_FRAMEBUFFER_OPERATION:"
                       " the framebuffer "
                       "object is not complete"
                    << std::endl;
                break;
            case GL_OUT_OF_MEMORY:
                std::cerr
                    << "GL_OUT_OF_MEMORY: "
                       "there is not enough memory left to "
                       "execute the command."
                    << std::endl;
                break;
        }
    }
}
// clang-format off

//---------------------------------------------------------
//---------------------------------------------------------

Vertex::Vertex()
{
    x = y = 0.f;
}

Vertex::Vertex(float x, float y)
{
    this->x = x;
    this->y = y;
}

Triangle::Triangle()
{
    triangle[0] = Vertex{};
    triangle[1] = Vertex{};
    triangle[2] = Vertex{};
}

// clang-format off
Triangle::Triangle(const Vertex& v1,
                   const Vertex& v2,
                   const Vertex& v3)
{
    triangle[0] = v1;
    triangle[1] = v2;
    triangle[2] = v3;
}
// clang-format on

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

// clang-format off
std::array<Keys, 6> keys_binds
{
    Keys
    {
        DevBinds::keypad,
        SDLK_w,
        Uint8(),
        "up",
        Events::up_pressed,
        Events::up_released
    },

    Keys
    {
        DevBinds::keypad,
        SDLK_UP,
        Uint8(),
        "up",
        Events::up_pressed,
        Events::up_released
    },

    Keys
    {
        DevBinds::keypad,
        SDLK_s,
        Uint8(),
        "down",
        Events::down_pressed,
        Events::down_released
    },

    Keys
    {
        DevBinds::keypad,
        SDLK_DOWN,
        Uint8(),
        "down",
        Events::down_pressed,
        Events::down_released
    },

    Keys
    {
        DevBinds::mouse,
        SDL_KeyCode(),
        SDL_BUTTON_LEFT,
        "left mouse's button",
        Events::mouse_left_pressed,
        Events::mouse_left_released
    },

    Keys
    {
        DevBinds::mouse,
        SDL_KeyCode(),
        SDL_BUTTON_RIGHT,
        "right mouse's button",
        Events::mouse_right_pressed,
        Events::mouse_right_released
    }
};
// clang-format on

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

bool check_input(const SDL_Event* const sdl_ev,
                 Keys&                  k,
                 DevBinds               db)
{
    auto res = keys_binds.end();
    if (db == DevBinds::keypad)
    { // 1
        res = std::find_if(keys_binds.begin(),
                           keys_binds.end(),
                           [&sdl_ev](const Keys& ks) -> bool {
                               return sdl_ev->key.keysym.sym ==
                                      ks.code_board;
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
                               return sdl_ev->button.button ==
                                      ks.code_mouse;
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
        std::cerr << "Error initialization "
                     "SDL libs! \n"
                  << SDL_GetError() << std::endl;
        return 1;
    }

    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                            SDL_GL_CONTEXT_DEBUG_FLAG))
    {
        std::cerr << "Error on setting debug "
                     "flag to context: "
                  << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow("Opengl minimal",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              window_width,
                              window_height,
                              SDL_WINDOW_OPENGL);

    if (window == nullptr)
    {
        std::cerr << "Error creating "
                     "window!\n";
        std::cerr << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    auto  gl_context_profile = SDL_GL_CONTEXT_PROFILE_CORE;
    GLint gl_major_ver       = 4;
    GLint gl_minor_ver       = 3;

    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            gl_context_profile))
    {
        std::cerr << "Error setting core "
                     "flag context: "
                  << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,
                            gl_major_ver))
    {
        std::cerr << "Error setting "
                     "major version "
                  << gl_major_ver
                  << " context: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,
                            gl_minor_ver))
    {
        std::cerr << "Error setting minor "
                     "value version "
                  << gl_minor_ver
                  << " context: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (gl_context == nullptr)
    {
        gl_major_ver = 3;
        gl_minor_ver = 2;
        if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                                gl_context_profile))
        {
            std::cerr << "Error setting core "
                         "flag context: "
                      << SDL_GetError() << std::endl;
            return EXIT_FAILURE;
        }

        if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,
                                gl_major_ver))
        {
            std::cerr << "Error setting "
                         "major version "
                      << gl_major_ver
                      << " context: " << SDL_GetError()
                      << std::endl;
            return EXIT_FAILURE;
        }

        if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,
                                gl_minor_ver))
        {
            std::cerr << "Error setting "
                         "minor value "
                         "version "
                      << gl_minor_ver
                      << " context: " << SDL_GetError()
                      << std::endl;
            return EXIT_FAILURE;
        }

        gl_context = SDL_GL_CreateContext(window);
        if (gl_context == nullptr)
        {
            std::cerr << "Error on creating "
                         "window context: "
                      << SDL_GetError() << std::endl;
            return EXIT_FAILURE;
        }
    }

    if (SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8))
    {
        std::cerr << "Error on setting 8 bit "
                     "flag to red color: "
                  << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    if (SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8))
    {
        std::cerr << "Error on setting 8 bit "
                     "flag to green color: "
                  << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    if (SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8))
    {
        std::cerr << "Error on setting 8 bit "
                     "flag to blue color: "
                  << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    int result = SDL_GL_GetAttribute(
        SDL_GL_CONTEXT_MAJOR_VERSION, &gl_major_ver);

    if (result != 0)
    {
        std::cerr << "Error on getting major "
                     "version value: "
                  << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    if (gl_major_ver < 3)
    {
        std::cerr << "Major version opengl "
                     "ES is too low. Need at "
                     "least opengl "
                     "es 3.2!"
                  << std::endl;
        return EXIT_FAILURE;
    }

    if (!gladLoadGLES2Loader(SDL_GL_GetProcAddress))
    {
        std::cerr << "Error getting pointers "
                     "to API OpenGl!"
                  << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGl " << gl_major_ver << "." << gl_minor_ver
              << std::endl;

    glEnable(GL_DEBUG_OUTPUT);
    my_gl_check();
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    my_gl_check();
    glDebugMessageCallback(callback_opengl_debug, nullptr);
    my_gl_check();
    glDebugMessageControl(GL_DONT_CARE,
                          GL_DONT_CARE,
                          GL_DONT_CARE,
                          0,
                          nullptr,
                          GL_TRUE);
    my_gl_check();

    time_begin = std::chrono::high_resolution_clock::now();

    return 0;
} // end init()

void Engine_impl::uninit()
{
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
            exit_success =
                check_input(&event, ks, DevBinds::keypad);
            ev = ks.key_pressed;
            break;

        case SDL_KEYUP:
            exit_success =
                check_input(&event, ks, DevBinds::keypad);
            ev = ks.key_released;
            break;

        case SDL_MOUSEBUTTONDOWN:
            exit_success =
                check_input(&event, ks, DevBinds::mouse);
            ev = ks.key_pressed;
            break;

        case SDL_MOUSEBUTTONUP:
            exit_success =
                check_input(&event, ks, DevBinds::mouse);
            ev = ks.key_released;
            break;
    }
    return exit_success;
}

void Engine_impl::update() {}

void Engine_impl::render_triangle(const Triangle& /*v*/)
{
    auto time_cur = std::chrono::high_resolution_clock::now();
    std::chrono::duration<GLfloat> delta_time =
        time_cur - time_begin;

    GLfloat col =
        0.5f * (std::sin(delta_time.count()) + 1.f);
    // std::cout << col << std::endl;

    glClearColor(col, 1.f - col, col, 0.f);
    my_gl_check();
    glClear(GL_COLOR_BUFFER_BIT);
    my_gl_check();
}

void Engine_impl::swap_buffers()
{
    SDL_GL_SwapWindow(window);
}

static const char* source_to_strv(GLenum source)
{
    switch (source)
    {
        case GL_DEBUG_SOURCE_API:
            return "API";
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            return "SHADER_COMPILER";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            return "WINDOW_SYSTEM";
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            return "THIRD_PARTY";
        case GL_DEBUG_SOURCE_APPLICATION:
            return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER:
            return "OTHER";
    }
    return "unknown";
}

static const char* type_to_strv(GLenum type)
{
    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
            return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            return "DEPRECATED_"
                   "BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            return "UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PERFORMANCE:
            return "PERFORMANCE";
        case GL_DEBUG_TYPE_PORTABILITY:
            return "PORTABILITY";
        case GL_DEBUG_TYPE_MARKER:
            return "MARKER";
        case GL_DEBUG_TYPE_PUSH_GROUP:
            return "PUSH_GROUP";
        case GL_DEBUG_TYPE_POP_GROUP:
            return "POP_GROUP";
        case GL_DEBUG_TYPE_OTHER:
            return "OTHER";
    }
    return "unknown";
}

static const char* severity_to_strv(GLenum severity)
{
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            return "HIGH";
        case GL_DEBUG_SEVERITY_MEDIUM:
            return "MEDIUM";
        case GL_DEBUG_SEVERITY_LOW:
            return "LOW";
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            return "NOTIFICATION";
    }
    return "unknown";
}

// 30Kb on my system, too much for stack
static std::array<char, GL_MAX_DEBUG_MESSAGE_LENGTH>
    local_log_buff;

static void APIENTRY
callback_opengl_debug(GLenum                       source,
                      GLenum                       type,
                      GLuint                       id,
                      GLenum                       severity,
                      GLsizei                      length,
                      const GLchar*                message,
                      [[maybe_unused]] const void* userParam)
{
    // The memory formessageis owned and
    // managed by the GL, and should
    // onlybe considered valid for the
    // duration of the function call.The
    // behavior of calling any GL or
    // window system function from
    // within thecallback function is
    // undefined and may lead to program
    // termination.Care must also be
    // taken in securing debug callbacks
    // for use with asynchronousdebug
    // output by multi-threaded GL
    // implementations.  Section 18.8
    // describes thisin further detail.

    auto& buff{ local_log_buff };
    int   num_chars = std::snprintf(buff.data(),
                                  buff.size(),
                                  "%s %s %d %s %.*s\n",
                                  source_to_strv(source),
                                  type_to_strv(type),
                                  id,
                                  severity_to_strv(severity),
                                  length,
                                  message);

    if (num_chars > 0)
    {
        // TODO use
        // https://en.cppreference.com/w/cpp/io/basic_osyncstream
        // to fix possible data races
        // now we use
        // GL_DEBUG_OUTPUT_SYNCHRONOUS
        // to garantie call in main
        // thread
        std::cerr.write(buff.data(), num_chars);
    }
}

} // namespace eng
