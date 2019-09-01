#include "../include/srf/system/app.h"

#include <SDL2/SDL.h>


void srf::system::SRFInitializer::Init()
{
    int err = SDL_Init(SDL_INIT_EVENTS);
    if (err < 0)
    {
        throw SDL_GetError();
    }

}

bool srf::system::SRFInitializer::IsInit()
{
    return SDL_WasInit(SDL_INIT_EVENTS);
}

void srf::system::SRFInitializer::Quit()
{
    SDL_Quit();
}


srf::system::Application::Application() : 
    _state(AppState::Idle)
{
    if (!SRFInitializer::IsInit())
    {
        SRFInitializer::Init();
    }
}

srf::system::Application::~Application()
{
    SRFInitializer::Quit();
}

void srf::system::Application::run()
{
    if (_state != AppState::Idle)
    {
        return;
    }
    _state = AppState::Run;

    onInit();
    onBegin();
    
    loop();

}

void srf::system::Application::pause()
{
    if (_state != AppState::Run)
    {
        return;
    }
    _state = AppState::Pause;
}

void srf::system::Application::resume()
{
    if (_state != AppState::Pause)
    {
        return;
    }
    _state = AppState::Run;

    onBegin();

    loop();
}

void srf::system::Application::stop()
{
    if (_state != AppState::Run
        && _state != AppState::Pause)
    {
        return;
    }
    _state = AppState::Stop;
}

void srf::system::Application::loop()
{
    while (_state == AppState::Run)
    {

        SDL_Event e;
        SDL_PumpEvents();
        while (SDL_PollEvent(&e) != NULL)
        {
            if (e.type == SDL_QUIT)
            {
                // this should be done on filter
                // but double check for safety
                stop();
                break;
            }



        }

        onLoop();
    }

    if (_state == AppState::Pause)
    {
        onPaused();
    }
    else if (_state == AppState::Stop)
    {
        onPaused();
        onStop();
    }
}
