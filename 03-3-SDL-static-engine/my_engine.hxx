#pragma once

namespace eng
{

enum class Events
{
    //    left_pressed,
    //    left_released,
    //    right_pressed,
    //    right_released,
    up_pressed,
    up_released,
    down_pressed,
    down_released,

    //    select_pressed,
    //    select_released,
    //    start_pressed,
    //    start_released,

    //    button1_X_pressed,
    //    button1_X_released,
    //    button2_A_pressed,
    //    button2_A_released,
    //    button3_Y_pressed,
    //    button3_Y_released,
    //    button4_B_pressed,
    //    button4_B_released,

    mouse_left_pressed,
    mouse_left_released,
    mouse_right_pressed,
    mouse_right_released,

    turn_off
};

class IEngine
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

IEngine* create_engine();
void     destroy_engine(IEngine*&);

} // namespace eng
