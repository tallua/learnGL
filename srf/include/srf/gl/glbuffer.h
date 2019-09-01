#pragma once

#include <array>
#include <vector>

#include "glbase.h"

namespace srf
{
namespace gl
{
class Buffer;

template<>
struct GLObjectHolder<Buffer>;

class Buffer
{
public:
    inline static std::vector<Buffer> Create(size_t size)
    {
        auto holder = std::make_shared<GLObjectHolder<Buffer>>(size);
        std::vector<Buffer> list;
        list.reserve(size);

        for (size_t index = 0; index < size; ++index)
        {
            list.push_back(Buffer(GLElementId<Buffer>(holder, index)));
        }

        return list;
    }

public:
    //inline static void Copy(Buffers from, size_t from_index, GLintptr from_offset,
    //    Buffers to, size_t to_index, GLintptr to_offset, GLsizeiptr count)
    //{
    //    // TODO
    //}
    //inline static void Copy(BufferStorages from, size_t from_index, GLintptr from_offset,
    //    Buffers to, size_t to_index, GLintptr to_offset, GLsizeiptr count)
    //{
    //    // TODO
    //}

public:
    enum class Target
    {
        Array = GL_ARRAY_BUFFER,
        AtomicCounter = GL_ATOMIC_COUNTER_BUFFER,
        CopyRead = GL_COPY_READ_BUFFER,
        CopyWrite = GL_COPY_WRITE_BUFFER,
        DispatchIndirect = GL_DISPATCH_INDIRECT_BUFFER,
        DrawIndirect = GL_DRAW_INDIRECT_BUFFER,
        ElementArray = GL_ELEMENT_ARRAY_BUFFER,
        PixelPack = GL_PIXEL_PACK_BUFFER,
        PixelUnpack = GL_PIXEL_UNPACK_BUFFER,
        Query = GL_QUERY_BUFFER,
        ShaderStorage = GL_SHADER_STORAGE_BUFFER,
        Texture = GL_TEXTURE_BUFFER,
        TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER,
        Uniform = GL_UNIFORM_BUFFER
    };

    enum class Usage
    {
        StaticDraw = GL_STATIC_DRAW,
        StaticRead = GL_STATIC_READ,
        StaticCopy = GL_STATIC_COPY,
        DynamicDraw = GL_DYNAMIC_DRAW,
        DynamicRead = GL_DYNAMIC_READ,
        DynamicCopy = GL_DYNAMIC_COPY,
        StreamDraw = GL_STREAM_DRAW,
        StreamRead = GL_STREAM_READ,
        StreamCopy = GL_STREAM_COPY
    };

public:

    Buffer()
    {
        auto holder = std::make_shared<GLObjectHolder<Buffer>>(1);
        _id = BufferId(holder, 0);
    }

    Buffer(const GLElementId<Buffer>& id)
        : _id(id)
    {

    }

    inline void bind(Target target)
    {
        glBindBuffer((GLenum)target, id());
    }

    inline void data(GLsizeiptr size_in_byte, void* data, Usage usage)
    {
        glNamedBufferData(id(), size_in_byte, data, (GLenum)usage);
    }

    template <typename _DATA_TYPE>
    inline void data(const _DATA_TYPE& data, Usage usage)
    {
        static_assert(false, "Uniform set doesnt support type");
    }


    inline void subdata(GLintptr offset, GLsizeiptr size_in_byte, void* data)
    {
        glNamedBufferSubData(id(), offset, size_in_byte, data);
    }

    inline void cleardata(size_t index)
    {
        // Why is this named 'clear'bufferdata?
    }

    inline GLuint id() const
    {
        return _id.id();
    }

private:

    using BufferId = GLElementId<Buffer>;
    BufferId _id;
};

#pragma region BUFFER
template <>
inline void Buffer::data(const std::vector<glm::vec2>& data, Buffer::Usage usage)
{
    glNamedBufferData(id(), data.size() * sizeof(glm::vec2), data.data(), (GLenum)usage);
}
template <>
inline void Buffer::data(const std::vector<glm::vec3>& data, Buffer::Usage usage)
{
    glNamedBufferData(id(), data.size() * sizeof(glm::vec3), data.data(), (GLenum)usage);
}
template <>
inline void Buffer::data(const std::vector<glm::vec4>& data, Buffer::Usage usage)
{
    glNamedBufferData(id(), data.size() * sizeof(glm::vec4), data.data(), (GLenum)usage);
}
#pragma endregion

template<>
struct GLObjectHolder<Buffer>
{
    GLObjectHolder(size_t size)
    {
        GLsizei arr_size = size;

        _ids.resize(size, 0);
        glCreateBuffers(arr_size, _ids.data());
    }

    ~GLObjectHolder()
    {
        if (_ids.size() == 0)
            return;

        glDeleteBuffers(_ids.size(), _ids.data());
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




// TODO
class MemoryMappedBuffer
{

};


}
}