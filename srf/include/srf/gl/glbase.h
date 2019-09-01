#pragma once

#include <Windows.h>
#include <gl/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace srf
{
namespace gl
{

class Uniform;
class Attribute;
class Program;

using Location = GLint;

//#pragma warning(push)
//#pragma warning(disable:4101)
//#pragma warning(pop)


template <typename _GLObject>
struct GLObjectHolder;


template <typename _GLObject>
class GLId final
{
public:
    GLId()
        : _holder(nullptr)
    {
        _id_cache = 0;
    }

    GLId(std::shared_ptr<GLObjectHolder<_GLObject>> holder)
        : _holder(holder)
    {
        _id_cache = _holder->id();
    }

    GLId(const GLId<_GLObject>& id)
        :_holder(id._holder)
    {
        _id_cache = _holder->id();
    }

    inline GLuint id() const { return _id_cache; }
private:
    std::shared_ptr<GLObjectHolder<_GLObject>> _holder;

    GLuint _id_cache;
};


template <typename _GLObject>
class GLElementId final
{
public:
    GLElementId()
        : _holder(nullptr), _index(0)
    {
        _id_cache = 0;
    }

    GLElementId(const std::shared_ptr<GLObjectHolder<_GLObject>>& holder, size_t index)
        : _holder(holder), _index(index)
    {
        _id_cache = _holder->id(_index);
    }

    GLElementId(const GLId<_GLObject>& id)
        :_holder(id._holder), _index(id._index)
    {
        _id_cache = _holder->id(_index);
    }

    inline GLuint id() const { return _id_cache; }
private:
    std::shared_ptr<GLObjectHolder<_GLObject>> _holder;
    size_t _index;

    GLuint _id_cache;
};


}
}