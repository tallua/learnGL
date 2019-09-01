#pragma once

#include <vector>

#include "glbase.h"

#include "gltype.h"
#include "glbuffer.h"


namespace srf
{
namespace gl
{

class Attribute;

class VertexArray;

template<>
struct GLObjectHolder<VertexArray>;

class VertexArray
{
public:
    inline static std::vector<VertexArray> Create(size_t size)
    {
        auto holder = std::make_shared<GLObjectHolder<VertexArray>>(size);
        std::vector<VertexArray> list;
        list.reserve(size);

        for (size_t index = 0; index < size; ++index)
        {
            list.push_back(VertexArray(GLElementId<VertexArray>(holder, index)));
        }

        return list;
    }

public:
    VertexArray()
    {
        auto holder = std::make_shared<GLObjectHolder<VertexArray>>(1);
        _id = VertexArrayId(holder, 0);
    }

    VertexArray(const GLElementId<VertexArray>& id)
        : _id(id)
    {

    }

    inline void bind()
    {
        glBindVertexArray(id());
    }

    template<typename T>
    inline void format(Attribute attrib, bool normalized, GLuint offset)
    {
        glVertexArrayAttribFormat(id(), attrib.loc(), 1, GLTypeParam<T>::val, normalized, offset);
    }

    template<>
    inline void format<glm::vec3>(Attribute attrib, bool normalized, GLuint offset)
    {
        glVertexArrayAttribFormat(id(), attrib.loc(), 3, GLTypeParam<float>::val, normalized, offset);
    }

    template<>
    inline void format<glm::vec4>(Attribute attrib, bool normalized, GLuint offset)
    {
        glVertexArrayAttribFormat(id(), attrib.loc(), 4, GLTypeParam<float>::val, normalized, offset);
    }


    inline void map(Attribute attrib, GLuint binding)
    {
        glVertexArrayAttribBinding(id(), attrib.loc(), binding);
    }

    inline void vertexBuffer(GLuint binding, Buffer buffer, GLintptr offset, GLsizei stride)
    {
        glVertexArrayVertexBuffer(id(), binding, buffer.id(), offset, stride);
    }

    inline void enable(const Attribute attrib)
    {
        glEnableVertexArrayAttrib(id(), attrib.loc());
    }

    inline void disable(const Attribute attrib)
    {
        glDisableVertexArrayAttrib(id(), attrib.loc());
    }

    inline GLuint id()
    {
        return _id.id();
    }

private:

    using VertexArrayId = GLElementId<VertexArray>;
    VertexArrayId _id;
};

template<>
struct GLObjectHolder<VertexArray>
{
    GLObjectHolder(size_t size)
    {
        GLsizei arr_size = size;

        _ids.resize(size, 0);
        glCreateVertexArrays(arr_size, _ids.data());
    }

    ~GLObjectHolder()
    {
        if (_ids.size() == 0)
            return;

        glDeleteVertexArrays(_ids.size(), _ids.data());
        _ids.clear();
    }

    inline GLuint id(size_t index) const
    {
        if (index < 0 || index >= _ids.size())
            return 0;

        return _ids[index];
    }

private:
    std::vector<GLuint> _ids;
};

}
}

