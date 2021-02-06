#include <chrono>
#include <iostream>

#include "my_engine.hxx"

class Aim : public eng::Game
{
private:
    // unsigned int num_aims;
public:
    Aim() {}
    void initialize() override {}
    bool on_event(eng::Events& ev) override
    {
        switch (ev)
        {
            case eng::Events::turn_off:
                std::cout << "1" << std::endl;
                return false;
                break;
        }
        return true;
    }
    void update() override {}
};

OM_DECLSPEC eng::Game* create_game(eng::IEngine* engine)
{
    if (engine != nullptr)
        return new Aim();

    return nullptr;
}

OM_DECLSPEC void destroy_game(eng::Game* game)
{
    if (game != nullptr)
        delete game;
}
