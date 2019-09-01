#pragma once

#include "../util/pointers.h"


namespace srf
{
namespace system
{


template <typename _Context, typename _Target>
class Renderer
{
public:
    virtual void renderStart(util::wptr<_Context> context) = 0;
    virtual void renderEnd(util::wptr<_Context> context) = 0;

    virtual void render(const _Target& target) = 0;
};


}
}


