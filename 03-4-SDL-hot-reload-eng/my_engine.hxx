#pragma once

#ifndef OM_DECLSPEC
#define OM_DECLSPEC
#endif

namespace eng
{

enum class Events
{
    up_pressed,
    up_released,
    down_pressed,
    down_released,

    mouse_left_pressed,
    mouse_left_released,
    mouse_right_pressed,
    mouse_right_released,
    turn_off
};

class OM_DECLSPEC IEngine
{
private:
public:
    virtual ~IEngine();
    virtual int  init()                      = 0;
    virtual void uninit()                    = 0;
    virtual bool read_input(eng::Events& ev) = 0;
    virtual bool update()                    = 0;
};

class Engine_impl : public eng::IEngine
{
public:
    int  init() override;
    void uninit() override;
    bool read_input(eng::Events& ev) override;
    bool update() override;
};

struct OM_DECLSPEC Game
{
    virtual ~Game()                   = default;
    virtual void initialize()         = 0;
    virtual bool on_event(Events& ev) = 0;
    virtual void update()             = 0;
    // virtual void render() const  = 0;
};

// struct OM_DECLSPEC AimTraining
//{
// private:
//    bool               end_game;
//    float              time_interval = 0.4f;
//    const int          AIM_WIDTH     = 40;
//    const int          AIM_HEIGHT    = 40;
//    const unsigned int MAX_AIMS      = 30;
//    unsigned int       num_aims;

// public:
//    ~AimTraining();
//    AimTraining();
//    void         on_event(eng::Events& ev);
//    void         update();
//    void         render() const;
//    bool         game_over() const;
//    float        get_time_interval() const;
//    int          get_aim_width() const;
//    int          get_aim_height() const;
//    unsigned int get_max_aims() const;
//    unsigned int get_num_aims() const;
//};

OM_DECLSPEC IEngine* create_engine();
OM_DECLSPEC void     destroy_engine(IEngine*&);

} // namespace eng

extern "C" OM_DECLSPEC eng::Game* create_game(eng::IEngine*);
extern "C" OM_DECLSPEC void       destroy_game(eng::Game*);
