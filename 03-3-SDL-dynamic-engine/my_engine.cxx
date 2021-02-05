#include <SDL.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <string_view>
#include <vector>

#include "my_engine.hxx"

namespace eng {

static bool engine_exists = false;
static unsigned int num_rects = 0;
std::chrono::duration<float> delta_time;
static std::chrono::time_point<std::chrono::high_resolution_clock> time_cur;
static std::chrono::time_point<std::chrono::high_resolution_clock> time_prev;

constexpr int WIDTH = 1024;
constexpr int HEIGHT = 768;
constexpr int AIM_WIDTH = 30;
constexpr int AIM_HEIGHT = 30;
constexpr unsigned int MAX_AIMS = 20;
constexpr float TIME_INTERVAL = 0.4f;

SDL_Window *window = nullptr;
SDL_Renderer *render = nullptr;

// struct Aim;
std::vector<SDL_Rect> v_rects;

enum class DevBinds { keypad, mouse };

struct Keys {
  DevBinds type;
  SDL_KeyCode code_board;
  Uint8 code_mouse;
  std::string_view name;
  Events key_pressed;
  Events key_released;
  Keys() {}
  Keys(DevBinds type, SDL_KeyCode code_board, Uint8 code_mouse,
       std::string_view name, Events kp, Events kr) {
    this->type = type;
    this->code_board = code_board;
    this->code_mouse = code_mouse;
    this->name = name;
    this->key_pressed = kp;
    this->key_released = kr;
  }
};

std::array<Keys, 6> keys_binds{
    Keys{DevBinds::keypad, SDLK_w, Uint8(), "up", Events::up_pressed,
         Events::up_released},
    Keys{DevBinds::keypad, SDLK_UP, Uint8(), "up", Events::up_pressed,
         Events::up_released},
    Keys{DevBinds::keypad, SDLK_s, Uint8(), "down", Events::down_pressed,
         Events::down_released},
    Keys{DevBinds::keypad, SDLK_DOWN, Uint8(), "down", Events::down_pressed,
         Events::down_released},
    Keys{DevBinds::mouse, SDL_KeyCode(), SDL_BUTTON_LEFT, "left mouse's button",
         Events::mouse_left_pressed, Events::mouse_left_released},
    Keys{DevBinds::mouse, SDL_KeyCode(), SDL_BUTTON_RIGHT,
         "right mouse's button", Events::mouse_right_pressed,
         Events::mouse_right_released}};

IEngine::~IEngine() {}

IEngine *create_engine() {
  if (engine_exists)
    return nullptr;
  IEngine *engine = new Engine_impl();
  engine_exists = true;
  return engine;
}

void destroy_engine(IEngine *&engine) {
  if (!engine_exists || engine == nullptr)
    return;
  engine_exists = false;
  delete engine;
  engine = nullptr;
}

void shoot(SDL_Event *ev) {
  auto res = std::find_if(
      v_rects.begin(), v_rects.end(), [&ev](const SDL_Rect &rect) -> bool {
        if (ev->button.x >= rect.x && ev->button.x <= rect.x + AIM_WIDTH) {
          if (ev->button.y >= rect.y && ev->button.y <= rect.y + AIM_HEIGHT)
            return true;
        }
        return false;
      });

  if (res != v_rects.end()) {
    v_rects.erase(res);
    num_rects--;
  }
}

bool check_input(const SDL_Event *const sdl_ev, Keys &k, DevBinds db) {
  auto res = keys_binds.end();
  if (db == DevBinds::keypad) { // 1
    res = std::find_if(keys_binds.begin(), keys_binds.end(),
                       [&sdl_ev](const Keys &ks) -> bool {
                         return sdl_ev->key.keysym.sym == ks.code_board;
                       });
    if (res == keys_binds.end())
      return false;
    else {
      k = *res;
      return true;
    }
  }      // 1
  else { // 2
    res = std::find_if(keys_binds.begin(), keys_binds.end(),
                       [&sdl_ev](const Keys &ks) -> bool {
                         return sdl_ev->button.button == ks.code_mouse;
                       });
    if (res == keys_binds.end())
      return false;
    else {
      k = *res;
      return true;
    }
  } // 2
}

int Engine_impl::init() {
  int res = SDL_Init(SDL_INIT_EVERYTHING);
  if (res) {
    std::cerr << "Error initialization SDL libs! \n"
              << SDL_GetError() << std::endl;
    return 1;
  }

  window = SDL_CreateWindow("Aim training", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
                            SDL_WINDOW_OPENGL);
  if (window == nullptr) {
    std::cerr << "Error creating window!\n";
    std::cerr << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (render == nullptr) {
    std::cerr << "Error creating render!" << std::endl;
    std::cerr << SDL_GetError() << std::endl;
    return 1;
  }

  v_rects.reserve(10);

  time_prev = std::chrono::high_resolution_clock::now();
  time_cur = std::chrono::high_resolution_clock::now();

  srand(time(NULL));

  return 0;
} // end init()

void Engine_impl::uninit() {
  SDL_DestroyRenderer(render);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

bool Engine_impl::read_input(Events &ev) {
  SDL_Event event;
  if (!SDL_PollEvent(&event))
    return false;
  bool exit_success = false;
  Keys ks;
  switch (event.type) {
  case SDL_QUIT:
    ev = Events::turn_off;
    exit_success = true;
    break;

  case SDL_KEYDOWN:
    exit_success = check_input(&event, ks, DevBinds::keypad);
    ev = ks.key_pressed;
    break;

  case SDL_KEYUP:
    exit_success = check_input(&event, ks, DevBinds::keypad);
    ev = ks.key_released;
    SDL_SetRenderDrawColor(render, 235, 230, 87, 255);
    break;

  case SDL_MOUSEBUTTONDOWN:
    exit_success = check_input(&event, ks, DevBinds::mouse);
    ev = ks.key_pressed;
    shoot(&event);
    break;

  case SDL_MOUSEBUTTONUP:
    exit_success = check_input(&event, ks, DevBinds::mouse);
    ev = ks.key_released;
    break;
  }
  return exit_success;
}

bool Engine_impl::update() {
  time_cur = std::chrono::high_resolution_clock::now();
  delta_time = time_cur - time_prev;
  if (delta_time.count() > TIME_INTERVAL) {
    if (num_rects >= MAX_AIMS) {
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
  for (const auto &el : v_rects) {
    // SDL_RenderPresent(render);
    //
    SDL_SetRenderDrawColor(render, 235, 230, 87, 255);
    SDL_RenderFillRect(render, &el);
  }
  SDL_RenderPresent(render);

  return true;
}

} // namespace eng
