#pragma once

#include "glbase.h"

#include "glprogram.h"

#include <vector>
#include <array>
#include <string>

namespace srf
{
namespace gl
{

class Uniform final
{
public:
    Uniform()
        : _program(), _loc(-1)
    {
    }

    Uniform(Program program, Location loc)
        : _program(program), _loc(loc)
    {

    }

    Uniform(Program program, const std::string& name)
        : _program(program), _loc(program.getUniform(name))
    {

    }

    template <typename _DATA_TYPE>
    inline void set(const _DATA_TYPE& data)
    {
        static_assert(false, "Uniform set doesnt support type");
    }

    //template <typename _DATA_TYPE, size_t _SIZE>
    //inline void set(const std::array<_DATA_TYPE, _SIZE>& arr)
    //{
    //    static_assert(false, "Uniform set doesnt support type");
    //}


    inline Location loc() const
    {
        return _loc;
    }
private:
    Program _program;
    Location _loc;
};

#pragma region UNIFORM_SET_1
// 1
template <>
inline void Uniform::set(const double& data)
{
    glProgramUniform1d(_program.id(), _loc, data);
}
template <>
inline void Uniform::set(const float& data)
{
    glProgramUniform1f(_program.id(), _loc, data);
}
template <>
inline void Uniform::set(const int& data)
{
    glProgramUniform1i(_program.id(), _loc, data);
}
template <>
inline void Uniform::set(const unsigned int& data)
{
    glProgramUniform1ui(_program.id(), _loc, data);
}
// 2
template <>
inline void Uniform::set(const glm::dvec2& data)
{
    glProgramUniform2d(_program.id(), _loc, data.x, data.y);
}
template <>
inline void Uniform::set(const glm::fvec2& data)
{
    glProgramUniform2f(_program.id(), _loc, data.x, data.y);
}
template <>
inline void Uniform::set(const glm::ivec2& data)
{
    glProgramUniform2i(_program.id(), _loc, data.x, data.y);
}
template <>
inline void Uniform::set(const glm::uvec2& data)
{
    glProgramUniform2ui(_program.id(), _loc, data.x, data.y);
}
// 3
template <>
inline void Uniform::set(const glm::dvec3& data)
{
    glProgramUniform3d(_program.id(), _loc, data.x, data.y, data.z);
}
template <>
inline void Uniform::set(const glm::fvec3& data)
{
    glProgramUniform3f(_program.id(), _loc, data.x, data.y, data.z);
}
template <>
inline void Uniform::set(const glm::ivec3& data)
{
    glProgramUniform3i(_program.id(), _loc, data.x, data.y, data.z);
}
template <>
inline void Uniform::set(const glm::uvec3& data)
{
    glProgramUniform3ui(_program.id(), _loc, data.x, data.y, data.z);
}
// 4
template <>
inline void Uniform::set(const glm::dvec4& data)
{
    glProgramUniform4d(_program.id(), _loc, data.x, data.y, data.z, data.w);
}
template <>
inline void Uniform::set(const glm::fvec4& data)
{
    glProgramUniform4f(_program.id(), _loc, data.x, data.y, data.z, data.w);
}
template <>
inline void Uniform::set(const glm::ivec4& data)
{
    glProgramUniform4i(_program.id(), _loc, data.x, data.y, data.z, data.w);
}
template <>
inline void Uniform::set(const glm::uvec4& data)
{
    glProgramUniform4ui(_program.id(), _loc, data.x, data.y, data.z, data.w);
}
// 1v
template <>
inline void Uniform::set(const std::vector<double>& data)
{
    glProgramUniform1dv(_program.id(), _loc, data.size(), data.data());
}
template <>
inline void Uniform::set(const std::vector<float>& data)
{
    glProgramUniform1fv(_program.id(), _loc, data.size(), data.data());
}
template <>
inline void Uniform::set(const std::vector<int>& data)
{
    glProgramUniform1iv(_program.id(), _loc, data.size(), data.data());
}
template <>
inline void Uniform::set(const std::vector<unsigned int>& data)
{
    glProgramUniform1uiv(_program.id(), _loc, data.size(), data.data());
}
// 2v
template <>
inline void Uniform::set(const std::vector<glm::dvec2>& data)
{
    glProgramUniform2dv(_program.id(), _loc, data.size(), glm::value_ptr(*(data.data())));
}
template <>
inline void Uniform::set(const std::vector<glm::fvec2>& data)
{
    glProgramUniform2fv(_program.id(), _loc, data.size(), glm::value_ptr(*(data.data())));
}
template <>
inline void Uniform::set(const std::vector<glm::ivec2>& data)
{
    glProgramUniform2iv(_program.id(), _loc, data.size(), glm::value_ptr(*(data.data())));
}
template <>
inline void Uniform::set(const std::vector<glm::uvec2>& data)
{
    glProgramUniform2uiv(_program.id(), _loc, data.size(), glm::value_ptr(*(data.data())));
}
// 3v
template <>
inline void Uniform::set(const std::vector<glm::dvec3>& data)
{
    glProgramUniform3dv(_program.id(), _loc, data.size(), glm::value_ptr(*(data.data())));
}
template <>
inline void Uniform::set(const std::vector<glm::fvec3>& data)
{
    glProgramUniform3fv(_program.id(), _loc, data.size(), glm::value_ptr(*(data.data())));
}
template <>
inline void Uniform::set(const std::vector<glm::ivec3>& data)
{
    glProgramUniform3iv(_program.id(), _loc, data.size(), glm::value_ptr(*(data.data())));
}
template <>
inline void Uniform::set(const std::vector<glm::uvec3>& data)
{
    glProgramUniform3uiv(_program.id(), _loc, data.size(), glm::value_ptr(*(data.data())));
}
// 4v
template <>
inline void Uniform::set(const std::vector<glm::dvec4>& data)
{
    glProgramUniform4dv(_program.id(), _loc, data.size(), glm::value_ptr(*(data.data())));
}
template <>
inline void Uniform::set(const std::vector<glm::fvec4>& data)
{
    glProgramUniform4fv(_program.id(), _loc, data.size(), glm::value_ptr(*(data.data())));
}
template <>
inline void Uniform::set(const std::vector<glm::ivec4>& data)
{
    glProgramUniform4iv(_program.id(), _loc, data.size(), glm::value_ptr(*(data.data())));
}
template <>
inline void Uniform::set(const std::vector<glm::uvec4>& data)
{
    glProgramUniform4uiv(_program.id(), _loc, data.size(), glm::value_ptr(*(data.data())));
}
#pragma endregion

#pragma region UNIFORM_SET_MATRIX
template <>
inline void Uniform::set(const glm::mat4& data)
{
    glProgramUniformMatrix4fv(_program.id(), _loc, 1,
        false, glm::value_ptr(data));
}


#pragma endregion


#pragma region UNIFORM_SET_2




#pragma endregion


class Attribute final
{
public:
    Attribute()
        : _loc(-1)
    {
    }

    Attribute(Location loc)
        : _loc(loc)
    {

    }

    Attribute(Program program, const std::string& name)
        : _loc(program.getAttribute(name))
    {

    }


    inline Location loc() const
    {
        return _loc;
    }

private:
    Location _loc;
};

}
}
