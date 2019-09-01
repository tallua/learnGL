#pragma once

#include "../util/pointers.h"
#include "../system/window.h"

#include "glbase.h"

namespace srf
{
namespace gl
{

// configuration for gl system
class GLContext final
{
public:
    GLContext();
    ~GLContext();

    void vsync(bool on);
    void depthTest(bool on);

    void clear();
    void clearColor(float r, float g, float b, float a);
    void clearColor(math::vec4 rgba);
    void viewport();

    GLuint error();
};

class GLWindow : public system::Window
{
public:
    GLWindow();
    GLWindow(const std::string& title, math::ivec2 pos, math::ivec2 size);
    virtual ~GLWindow();

    virtual bool valid() const override
    {
        if (!Window::valid())
            return false;
        if (_sdl_context == nullptr)
            return false;
        return true;
    }

    virtual void setSize(const math::ivec2& new_size) override;

    void makeCurrent();
    void swapWindow();

    util::wptr<GLContext> context() { return _context; }

protected:
    GLWindow(std::uint32_t sdl_flags,
        const std::string& title, math::ivec2 pos, math::ivec2 size);

    util::sptr<GLContext> _context;
    void* _sdl_context;
};

}
}