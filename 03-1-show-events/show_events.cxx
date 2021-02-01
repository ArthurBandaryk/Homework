#include <SDL.h>
//#include <SDL_gamecontroller.h>

#include <iostream>
#include <string_view>
#include <array>
#include <algorithm>


struct KeypadKey //bind keypad key code with string name
{
    SDL_Keycode code;
    std::string_view name;
    KeypadKey(const SDL_Keycode &code,
              const std::string_view name)
    {
        this->code = code;
        this->name = name;
    }
};

struct JoyKey //bind joystick button code with string name
{
    SDL_GameControllerButton code_but;
    std::string_view name;
    JoyKey(const SDL_GameControllerButton code_but,
           const std::string_view name)
    {
        this->code_but = code_but;
        this->name = name;
    }
};


std::array<KeypadKey, 15> keys_bind
{
    //keypad definition
    KeypadKey(SDLK_w, "w"),
    KeypadKey(SDLK_s, "s"),
    KeypadKey(SDLK_a, "a"),
    KeypadKey(SDLK_d, "d"),

    KeypadKey(SDLK_UP, "up"),
    KeypadKey(SDLK_DOWN, "down"),
    KeypadKey(SDLK_LEFT, "left"),
    KeypadKey(SDLK_RIGHT, "right"),

    KeypadKey(SDLK_SPACE, "space"),
    KeypadKey(SDLK_LSHIFT, "left shift"),
    KeypadKey(SDLK_LCTRL, "left ctrl"),
    KeypadKey(SDLK_LALT, "left alt"),

    KeypadKey(SDLK_RSHIFT, "right shift"),
    KeypadKey(SDLK_RCTRL, "right ctrl"),
    KeypadKey(SDLK_RALT, "right alt")
};

std::array<JoyKey, 10> joyst_bind
{
    JoyKey(SDL_CONTROLLER_BUTTON_A, "A"),
    JoyKey(SDL_CONTROLLER_BUTTON_B, "B"),
    JoyKey(SDL_CONTROLLER_BUTTON_X, "X"),
    JoyKey(SDL_CONTROLLER_BUTTON_Y, "Y"),

    JoyKey(SDL_CONTROLLER_BUTTON_DPAD_UP, "Up"),
    JoyKey(SDL_CONTROLLER_BUTTON_DPAD_DOWN, "Down"),
    JoyKey(SDL_CONTROLLER_BUTTON_DPAD_LEFT, "Left"),
    JoyKey(SDL_CONTROLLER_BUTTON_DPAD_RIGHT, "Right"),

    JoyKey(SDL_CONTROLLER_BUTTON_BACK, "Back"),
    JoyKey(SDL_CONTROLLER_BUTTON_START, "Start")
};

void check_input(const SDL_Event* const event)
{
    auto res_keypad = keys_bind.end(); //for keypad events
    auto res_joyst  = joyst_bind.end(); //for joystick events

    if(event->type == SDL_KEYDOWN ||
       event->type == SDL_KEYUP)
    { //1 keypad events
       res_keypad = std::find_if(keys_bind.begin(), keys_bind.end(),
                    [&event](const KeypadKey & k)->bool
                    {
                        return k.code == event->key.keysym.sym;
                    });


       if(res_keypad != keys_bind.end()) //we found specific keypad key
       {//2
           std::cout << "Keyboard key {" << res_keypad->name << "}";
           if(event->type == SDL_KEYDOWN)
           {
               std::cout << " was pressed!!! " << std::endl;
           }
           if(event->type == SDL_KEYUP)
           {
               std::cout << " was released!!!" << std::endl;
           }
       }//2
    }//1 end keypad


    if(event->type == SDL_CONTROLLERBUTTONDOWN ||
       event->type == SDL_CONTROLLERBUTTONUP)
    {//1 joystick events
       res_joyst = std::find_if(joyst_bind.begin(), joyst_bind.end(),
                   [&event](const JoyKey & cb)->bool
                   {
                        return cb.code_but == event->cbutton.button;
                   });

       if(res_joyst != joyst_bind.end()) //we found specific joystick key
       {//2
           std::cout << "Joystick key {" << res_joyst->name << "}";
           if(event->type == SDL_CONTROLLERBUTTONDOWN)
           {
               std::cout << " was pressed!!! " << std::endl;
           }
           if(event->type == SDL_CONTROLLERBUTTONUP)
           {
               std::cout << " was released!!!" << std::endl;
           }
       }//2
    }//1 end joystick


}// end check_input function



int main(int /*argc*/, char */*argv*/[])
{
    int res = SDL_Init(SDL_INIT_GAMECONTROLLER);
    if(res)
    {
        std::cerr << "Error initialisation SDL libs: "
                  << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    SDL_Window *window = nullptr;
    window             = SDL_CreateWindow("Show events",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              640, 480,
                              SDL_WINDOW_OPENGL);

    if(window == nullptr)
    {
        std::cerr << "Error on creating window. \n"
                  << SDL_GetError() << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    auto num_joysticks = SDL_NumJoysticks();
    std::cout << "Number of joysticks attached to the system: "
              << num_joysticks << std::endl;

    SDL_GameController *game_controller = nullptr;

    if(num_joysticks > 0)
    {//1
        for(int i = 0; i < num_joysticks; ++i)
        {//2
            if(SDL_IsGameController(i))
            {//3
                std::cout << "Joystick[" << i
                          << "] is supported by game controller interface."
                          << std::endl;
                game_controller = SDL_GameControllerOpen(i);
                if(game_controller == nullptr)
                {
                    std::cerr << "Error opening controller: "
                              << SDL_GetError() << std::endl;
                }
                else break;

            }//3
            else
            {//4
                std::cout << "Joystick[" << i
                          << "] isn't supported by game controller interface."
                          << std::endl;
            }//4
        }//2
    }//1


    if(!SDL_GameControllerGetAttached(game_controller))
    {
        std::cout << "No game controller has been attached!" << std::endl;
    }
    else
    {
        std::cout << "Game controller is opened and attached to system!"
                  << std::endl;
    }


    bool loop_continue = true;

    while(loop_continue)
    {//1
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {//2
            switch (event.type)
            {//3
                case SDL_KEYUP:
                check_input(&event);
                break;

                case SDL_KEYDOWN:
                check_input(&event);
                break;

                case SDL_CONTROLLERBUTTONDOWN:
                check_input(&event);
                break;

                case SDL_CONTROLLERBUTTONUP:
                check_input(&event);
                break;

                case SDL_QUIT:
                loop_continue = false;
                break;
            }//3
        }//2
        //std::cout << "Event queue is empty!" << std::endl;
    }//1

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

