#pragma once

#include "glbase.h"

namespace srf
{
namespace gl
{

template <typename T>
struct GLTypeParam;

template <>
struct GLTypeParam<GLbyte>
{
    enum { val = GL_BYTE };
};

template <>
struct GLTypeParam<GLubyte>
{
    enum { val = GL_UNSIGNED_BYTE };
};

template <>
struct GLTypeParam<GLshort>
{
    enum { val = GL_SHORT };
};

template <>
struct GLTypeParam<GLushort>
{
    enum { val = GL_UNSIGNED_SHORT };
};

template <>
struct GLTypeParam<GLint>
{
    enum { val = GL_INT };
};

template <>
struct GLTypeParam<GLuint>
{
    enum { val = GL_UNSIGNED_INT };
};

template <>
struct GLTypeParam<GLfloat>
{
    enum { val = GL_FLOAT };
};

template <>
struct GLTypeParam<GLdouble>
{
    enum { val = GL_DOUBLE };
};



}
}

