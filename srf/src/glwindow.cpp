#include "../include/srf/gl/glwindow.h"

#include <SDL2/SDL.h>

#include <Windows.h>
#include <gl/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>


srf::gl::GLContext::GLContext()
{
    glewExperimental = true;
    glewInit();
}

srf::gl::GLContext::~GLContext()
{
}

void srf::gl::GLContext::vsync(bool on)
{
    if (on)
        SDL_GL_SetSwapInterval(1);
    else
        SDL_GL_SetSwapInterval(0);
}

void srf::gl::GLContext::depthTest(bool on)
{
    if (on)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
}

void srf::gl::GLContext::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void srf::gl::GLContext::clearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void srf::gl::GLContext::clearColor(math::vec4 rgba)
{
    glClearColor(rgba.r, rgba.g, rgba.b, rgba.a);
}


srf::gl::GLWindow::GLWindow() : 
    GLWindow("srf glwindow",
        math::ivec2(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED),
        math::ivec2(800, 600))
{

}

srf::gl::GLWindow::GLWindow(const std::string& title, math::ivec2 pos, math::ivec2 size) : 
    GLWindow(SDL_WINDOW_OPENGL, title, pos, size)
{

}


srf::gl::GLWindow::GLWindow(std::uint32_t sdl_flags, const std::string& title, math::ivec2 pos, math::ivec2 size) : 
    Window(sdl_flags | SDL_WINDOW_OPENGL, title, pos, size),
    _sdl_context(nullptr),
    _context(nullptr)
{
    SDL_GLContext gl_context = SDL_GL_CreateContext(static_cast<SDL_Window*>(_window_ptr));
    if (gl_context == nullptr)
    {
        throw SDL_GetError();
    }

    _sdl_context = gl_context;
    _context = std::make_shared<GLContext>();
}

srf::gl::GLWindow::~GLWindow()
{
    if (_sdl_context)
    {
        SDL_GL_DeleteContext(static_cast<SDL_GLContext>(_sdl_context));
        _sdl_context = nullptr;
    }

}

void srf::gl::GLWindow::setSize(const math::ivec2& new_size)
{
    Window::setSize(new_size);

    if (valid())
    {
        // save previous state.
        auto prev_window = SDL_GL_GetCurrentWindow();
        auto prev_context = SDL_GL_GetCurrentContext();

        // resize viewport
        makeCurrent();
        glViewport(0, 0, new_size.x, new_size.y);

        // restore previous state.
        SDL_GL_MakeCurrent(prev_window, prev_context);
    }
}

void srf::gl::GLWindow::makeCurrent()
{
    if (valid())
    {
        SDL_GL_MakeCurrent(static_cast<SDL_Window*>(_window_ptr), 
            static_cast<SDL_GLContext>(_sdl_context));
    }
}

void srf::gl::GLWindow::swapWindow()
{
    if (valid())
    {
        SDL_GL_SwapWindow(static_cast<SDL_Window*>(_window_ptr));
    }
}

