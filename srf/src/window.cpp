#include "../include/srf/system/window.h"

#include <SDL2/SDL.h>

using namespace srf::math;

srf::system::Window::Window()
    : Window("srf window", 
        math::ivec2(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED), 
        math::ivec2(800, 600))
{

}

srf::system::Window::Window(const std::string& title, math::ivec2 pos, math::ivec2 size)
    : Window(SDL_WINDOW_SHOWN, title, pos, size)
{

}


srf::system::Window::Window(std::uint32_t sdl_flags,
    const std::string& title, math::ivec2 pos, math::ivec2 size)
    : _window_ptr(nullptr)
{

    SDL_Window* window = SDL_CreateWindow(
        title.c_str(),
        pos.x, pos.y,
        size.x, size.y, sdl_flags);

    if (window == nullptr)
    {
        throw SDL_GetError();
    }

    _window_ptr = window;
    
}


srf::system::Window::~Window()
{
    if (_window_ptr)
    {
        SDL_DestroyWindow(static_cast<SDL_Window*>(_window_ptr));
        _window_ptr = nullptr;
    }
}

const std::string srf::system::Window::title() const
{
    const char* title = SDL_GetWindowTitle(static_cast<SDL_Window*>(_window_ptr));

    return std::string(title);
}

void srf::system::Window::setTitle(const std::string& new_title)
{
    if (new_title.length() == 0)
    {
        return;
    }

    if (valid())
    {
        SDL_SetWindowTitle(static_cast<SDL_Window*>(_window_ptr), new_title.c_str());
    }
}

const srf::math::ivec2 srf::system::Window::size() const
{
    int width = 0, height = 0;

    if (valid())
    {
        SDL_GetWindowSize(static_cast<SDL_Window*>(_window_ptr), &width, &height);
    }

    return ivec2(width, height);
}

void srf::system::Window::setSize(const math::ivec2& new_size)
{
    if (valid())
    {
        SDL_SetWindowSize(static_cast<SDL_Window*>(_window_ptr), new_size.x, new_size.y);
    }
}

const srf::math::ivec2 srf::system::Window::pos() const
{
    int posx = 0, posy = 0;

    if (valid())
    {
        SDL_GetWindowPosition(static_cast<SDL_Window*>(_window_ptr), &posx, &posy);
    }

    return ivec2(posx, posy);
}

void srf::system::Window::setPos(const math::ivec2& new_pos)
{
    if (valid())
    {
        SDL_SetWindowSize(static_cast<SDL_Window*>(_window_ptr), new_pos.x, new_pos.y);
    }
}

const bool srf::system::Window::activate() const
{
    if (valid())
    {
        Uint32 flag = SDL_GetWindowFlags(static_cast<SDL_Window*>(_window_ptr));

        if (flag & SDL_WINDOW_SHOWN)
        {
            return true;
        }
        else if (flag & SDL_WINDOW_HIDDEN)
        {
            return false;
        }
    }

    return false;
}

void srf::system::Window::setActivate(bool activate)
{
    if (valid())
    {
        if (activate)
        {
            SDL_ShowWindow(static_cast<SDL_Window*>(_window_ptr));
        }
        else
        {
            SDL_HideWindow(static_cast<SDL_Window*>(_window_ptr));
        }
    }
}

bool srf::system::Window::focused() const
{
    if (valid())
    {
        Uint32 flag = SDL_GetWindowFlags(static_cast<SDL_Window*>(_window_ptr));

        if (flag & SDL_WINDOW_INPUT_FOCUS)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}

void srf::system::Window::tryFocus()
{
    if (valid())
    {
        SDL_RaiseWindow(static_cast<SDL_Window*>(_window_ptr));
    }
}

void srf::system::Window::setIcon()
{


}


