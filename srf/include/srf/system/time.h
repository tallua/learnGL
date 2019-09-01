#pragma once

namespace srf
{
namespace system
{



class Time
{
public:

    void start();

    void framecheck();

    inline unsigned int deltaTimeMS() { return _delta_tick; }
    inline float deltaTime() { return deltaTimeMS() / 1000.0f; }

    inline unsigned int totalTimeMS() { return _last_tick - _start_tick; }
    inline float totalTime() { return totalTimeMS() / 1000.0f; }

private:
    unsigned int _start_tick;
    unsigned int _last_tick;
    unsigned int _delta_tick;
};

}
}