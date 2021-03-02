#include <SDL.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>

#include "gfx_program.hxx"
#include "glad/glad.h"
#include "my_engine.hxx"
#include "picopng.hxx"

namespace eng
{

static bool engine_exists = false;

SDL_Window*   window     = nullptr;
SDL_GLContext gl_context = nullptr;
// GLuint        program_object;
std::vector<gfx_program> v_progs;

constexpr GLuint window_width  = 800;
constexpr GLuint window_height = 600;

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

//---------------------------------------------------------
//---------------------------------------------------------

Vertex::Vertex()
{
    x = y = tx = ty = 0.f;
}

// clang-format off
Vertex::Vertex(float x,  float y,
               float tx, float ty)
{
    this->x = x;
    this->y = y;
    this->tx = tx;
    this->ty = ty;
}
// clang-format on

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

Vertex interpolate(const Vertex& v1,
                   const Vertex& v2,
                   const float   t)
{
    Vertex result;
    result.x  = v1.x + (v2.x - v1.x) * t;
    result.y  = v1.y + (v2.y - v1.y) * t;
    result.tx = 0.f;
    result.ty = 0.f;
    return result;
}

Triangle interpolate(const Triangle& tr1,
                     const Triangle& tr2,
                     const float     t)
{
    Vertex v1 =
        interpolate(tr1.triangle[0], tr2.triangle[0], t);
    Vertex v2 =
        interpolate(tr1.triangle[1], tr2.triangle[1], t);
    Vertex v3 =
        interpolate(tr1.triangle[2], tr2.triangle[2], t);

    return Triangle{ v1, v2, v3 };
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

    window = SDL_CreateWindow("Vertex morphing",
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
    GLint gl_major_ver       = 3;
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
                  << " context: " << SDL_GetError()
                  << std::endl;
        return EXIT_FAILURE;
    }

    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,
                            gl_minor_ver))
    {
        std::cerr << "Error setting minor "
                     "value version "
                  << gl_minor_ver
                  << " context: " << SDL_GetError()
                  << std::endl;
        return EXIT_FAILURE;
    }

    gl_context = SDL_GL_CreateContext(window);

    int result = SDL_GL_GetAttribute(
        SDL_GL_CONTEXT_MAJOR_VERSION, &gl_major_ver);

    if (result != 0)
    {
        std::cerr << "Error on getting major "
                     "version value: "
                  << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    if (gl_major_ver <= 2)
    {
        std::cerr << "Major version opengl "
                     "ES is too low. Need at "
                     "least opengl "
                     "es 2.2!"
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

    std::cout << "OpenGl " << gl_major_ver << "."
              << gl_minor_ver << std::endl;

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

    // generate vbo
    GLuint vertex_vbo;
    glGenBuffers(1, &vertex_vbo);
    my_gl_check();

    // activate vbo
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    my_gl_check();

    // generate vao
    GLuint index_vao;
    glGenVertexArrays(1, &index_vao);
    my_gl_check();

    // activate generated vao
    glBindVertexArray(index_vao);
    my_gl_check();

    gfx_program prog1;
    if (prog1.prog_init("../shader_source/vert1.txt", "../shader_source/frag1.txt"))
        return EXIT_FAILURE;

    gfx_program prog2;
    if (prog2.prog_init("../shader_source/vert_text.txt", "../shader_source/frag_text.txt"))
        return EXIT_FAILURE;

    if (!prog1.is_validated())
        return EXIT_FAILURE;

    if (!prog2.is_validated())
        return EXIT_FAILURE;

    v_progs.push_back(prog1);
    v_progs.push_back(prog2);

    glUseProgram(prog2.get_id_program());
    my_gl_check();

    int location = glGetUniformLocation(
        v_progs[1].get_id_program(), "s_texture");
    my_gl_check();

    int texture_unit = 0;
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    my_gl_check();

    if (!load_texture("../resources/tank.png"))
    {
        std::cerr << "error loading texture!" << std::endl;
        return EXIT_FAILURE;
    }

    glUniform1i(location, 0 + texture_unit);
    my_gl_check();

    glEnable(GL_BLEND);
    my_gl_check();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    my_gl_check();

    // glEnable(GL_DEPTH_TEST);
    // my_gl_check();
    time_begin = std::chrono::high_resolution_clock::now();

    return EXIT_SUCCESS;
} // end init()

void Engine_impl::uninit()
{
    for (auto& el : v_progs)
    {
        glDeleteShader(el.get_vertex_shader_id());
        my_gl_check();
        glDeleteShader(el.get_fragment_shader_id());
        my_gl_check();
        glDeleteProgram(el.get_id_program());
        my_gl_check();
    }
    v_progs.clear();
    SDL_GL_DeleteContext(gl_context);
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

void Engine_impl::render_triangle(const Triangle& t)
{
    // glUseProgram(v_progs[1].get_id_program());
    // my_gl_check();

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(t.triangle),
        reinterpret_cast<const GLvoid*>(t.triangle.data()),
        GL_STATIC_DRAW);
    my_gl_check();

    glEnableVertexAttribArray(0);
    my_gl_check();

    GLintptr offset_attr_pos = 0;
    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<const void*>(offset_attr_pos));
    my_gl_check();

    glEnableVertexAttribArray(1);
    my_gl_check();

    GLintptr color_attr_offset = sizeof(float) * 2;
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<const void*>(color_attr_offset));
    my_gl_check();

    glDrawArrays(GL_TRIANGLES, 0, 3);
    my_gl_check();

    glDisableVertexAttribArray(0);
    my_gl_check();
}

void Engine_impl::swap_buffers()
{
    SDL_GL_SwapWindow(window);

    glClearColor(1.f, 0.5f, 0.2f, 0.f);
    my_gl_check();
    glClear(GL_COLOR_BUFFER_BIT);
    my_gl_check();
}

float Engine_impl::get_time_from_init()
{
    decltype(
        std::chrono::high_resolution_clock::now()) time_cur;
    time_cur = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> delta_time =
        time_cur - time_begin;
    return delta_time.count();
}

bool Engine_impl::load_texture(std::string_view path)
{
    std::vector<std::byte> png_in_mem;
    std::ifstream          fin;
    fin.open(path.data(), std::ios::binary);
    if (!fin.is_open())
    {
        std::cerr << "Error opening file " << path
                  << std::endl;
        return false;
    }
    else
    {
        std::vector<std::byte> img_in_pixels;
        std::streamsize        size_buf;
        fin.seekg(0, std::ios::end);
        size_buf = static_cast<std::streamsize>(fin.tellg());

        if (!size_buf)
        {
            std::cerr << "Nothing to decode!" << std::endl;
            fin.close();
            return false;
        }

        fin.seekg(0, std::ios::beg);
        png_in_mem.resize(size_buf);

        fin.read(reinterpret_cast<char*>(png_in_mem.data()),
                 size_buf);

        unsigned long img_w, img_h;
        img_h = img_w = 0;
        int err       = decodePNG(img_in_pixels,
                            img_w,
                            img_h,
                            png_in_mem.data(),
                            png_in_mem.size(),
                            false);
        if (err)
        {
            std::cerr << "Error decoding png: " << err
                      << std::endl;
            fin.close();
            return false;
        }

        GLuint texture_id = 0;
        glGenTextures(1, &texture_id);
        my_gl_check();

        glBindTexture(GL_TEXTURE_2D, texture_id);
        my_gl_check();

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA,
                     static_cast<GLsizei>(img_w),
                     static_cast<GLsizei>(img_h),
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     img_in_pixels.data());
        my_gl_check();

        glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        my_gl_check();
        glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        my_gl_check();

        // glBindTexture(GL_TEXTURE_2D, 0);
        // my_gl_check();

        fin.close();
    }
    return true;
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
