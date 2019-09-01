#pragma once

#include <string>
#include "../math/include_glm.h"

namespace srf
{
namespace system
{

class Window
{
public:
    Window();
    Window(const std::string& title, math::ivec2 pos, math::ivec2 size);
    virtual ~Window();

    // properties
    virtual bool valid() const { return _window_ptr != nullptr; }

    // get title of window.
    const std::string title() const;
    virtual void setTitle(const std::string& new_title);
    // get size of window.
    const math::ivec2 size() const;
    virtual void setSize(const math::ivec2& new_size);
    // get position of window.
    const math::ivec2 pos() const;
    virtual void setPos(const math::ivec2& new_pos);
    // get activation of window.
    const bool activate() const;
    virtual void setActivate(bool activate);

    // get if window is focused.
    bool focused() const;
    void tryFocus();
    // set icon of window.
    void setIcon();

protected:
    /// need an sdl flags to create proper window
    Window(std::uint32_t sdl_flags,
        const std::string& title,math::ivec2 pos, math::ivec2 size);

    void* _window_ptr;

};



}
}
