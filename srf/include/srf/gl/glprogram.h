#pragma once

#include <string>
#include <array>
#include <vector>
#include <memory>

#include "glbase.h"

namespace srf
{

namespace gl
{

#pragma region SHADER
class Shader;

template<>
struct GLObjectHolder<Shader>;

class Shader
{
public:
    static void ReleaseCompiler()
    {
        glReleaseShaderCompiler();
    }
public:
    enum class Type
    {
        Vertex = GL_VERTEX_SHADER,
        Fragment = GL_FRAGMENT_SHADER,
        Compute = GL_COMPUTE_SHADER,
        TessControl = GL_TESS_CONTROL_SHADER,
        TessEval = GL_TESS_EVALUATION_SHADER,
        Geometry = GL_GEOMETRY_SHADER
    };
public:

    inline void create(Type shader_type)
    {
        auto holder = std::make_shared<GLObjectHolder<Shader>>(shader_type);
        _id = ShaderId(holder);
    }

    inline void destroy()
    {
        _id = ShaderId();
    }

    inline void source(const int count, const char* const* src, const int* length)
    {
        glShaderSource(id(), count, src, length);
    }

    inline void source(const std::string& src)
    {
        int src_length = src.length();
        const char* src_str = src.c_str();

        glShaderSource(id(), 1, &src_str, &src_length);
    }

    inline void source(const std::vector<std::string>& src)
    {
        int cnt = src.size();
        std::vector<const char*> c_src;
        std::vector<int> len_src;

        c_src.reserve(cnt);
        len_src.reserve(cnt);

        for (auto& s : src)
        {
            c_src.push_back(s.c_str());
            len_src.push_back(s.length());
        }

        glShaderSource(id(), cnt, c_src.data(), len_src.data());
    }

    inline void compile()
    {
        glCompileShader(id());
    }

    inline bool compileStatus() const
    {
        int compiled = GL_FALSE;
        glGetShaderiv(id(), GL_COMPILE_STATUS, &compiled);

        if (compiled == GL_FALSE)
            return false;
        return true;
    }

    inline bool deleteStatus() const
    {
        int deleted = GL_FALSE;
        glGetShaderiv(id(), GL_DELETE_STATUS, &deleted);

        if (deleted == GL_FALSE)
            return false;
        return true;
    }

    inline Shader::Type type() const
    {
        GLint type = GL_NONE;
        glGetShaderiv(id(), GL_SHADER_TYPE, &type);

        return (Shader::Type)(type);
    }

    inline std::string infolog() const
    {
        if (id() == 0)
            return "Shader creation failed";

        GLint length = 0;
        glGetShaderiv(id(), GL_INFO_LOG_LENGTH, &length);

        if (length == 0)
            return std::string("");

        char* msg = new char[length];
        glGetShaderInfoLog(id(), length, &length, msg);

        std::string msg_str(msg);

        delete[] msg;

        return msg_str;
    }

    inline GLuint id() const 
    {
        return _id.id();
    }

private:
    using ShaderId = GLId<Shader>;
    ShaderId _id;
};


template<>
struct GLObjectHolder<Shader>
{
public:
    GLObjectHolder(Shader::Type type)
    {
        _id = glCreateShader((GLenum)type);
    }

    ~GLObjectHolder()
    {
        if (_id == 0)
            return;

        glDeleteShader(_id);
        _id = 0;
    }

    inline GLuint id() const 
    { 
        return _id; 
    }

private:
    GLuint _id;
};

#pragma endregion

#pragma region PROGRAM

class Program;

template<>
struct GLObjectHolder<Program>;

using ProgramId = GLId<Program>;

class Program
{
public:
    static void UnuseAll()
    {
        glUseProgram(0);
    }


public:
    inline void create()
    {
        auto holder = std::make_shared<GLObjectHolder<Program>>();
        _id = ProgramId(holder);
    }
    
    inline void destroy()
    {
        _id = ProgramId();
    }

    inline void attach(const Shader& shader)
    {
        glAttachShader(id(), shader.id());
    }

    inline void detach(const Shader& shader)
    {
        glDetachShader(id(), shader.id());
    }

    inline void link()
    {
        glLinkProgram(id());
    }

    inline bool linkStatus() const
    {
        int linked = GL_FALSE;
        glGetProgramiv(id(), GL_LINK_STATUS, &linked);

        if (linked == GL_FALSE)
            return false;

        return true;
    }

    inline std::string infoLog() const
    {
        if (id() == 0)
            return "Program creation failed";

        GLint length;
        glGetProgramiv(id(), GL_INFO_LOG_LENGTH, &length);

        if (length == 0)
            return std::string("");

        char* msg = new char[length];
        glGetProgramInfoLog(id(), length, &length, msg);

        std::string msg_str(msg);

        delete[] msg;
        
        return msg_str;
    }

    // TODO
    inline Location getAttribute(const std::string& name)
    {
        Location loc = glGetAttribLocation(id(), name.c_str());

        return loc;
    }

    inline Location getUniform(const std::string& name)
    {
        Location loc = glGetUniformLocation(id(), name.c_str());

        return loc;
    }
    // TODO
    //inline std::vector<Location> getUniforms(const std::vector<std::string>& names);

    inline void use()
    {
        glUseProgram(id());
    }


    inline GLuint id() const
    { 
        return _id.id(); 
    }
private:
    ProgramId _id;
};


template<>
struct GLObjectHolder<Program>
{
public:
    GLObjectHolder()
    {
        _id = glCreateProgram();
    }

    ~GLObjectHolder()
    {
        if (_id == 0)
            return;

        glDeleteProgram(_id);
        _id = 0;
    }

    inline GLuint id() const 
    { 
        return _id; 
    }

private:
    GLuint _id;
};

#pragma endregion

#pragma region PIPELINE

class Pipeline;

template<>
struct GLObjectHolder<Pipeline>;

class Pipeline
{
public:
    inline static std::vector<Pipeline> Create(size_t size)
    {
        auto holder = std::make_shared<GLObjectHolder<Pipeline>>(size);
        std::vector<Pipeline> list;
        list.reserve(size);

        for (size_t index = 0; index < size; ++index)
        {
            list.push_back(Pipeline(GLElementId<Pipeline>(holder, index)));
        }

        return list;
    }

public:
    enum class Type
    {
        Vertex = GL_VERTEX_SHADER_BIT,
        Fragment = GL_FRAGMENT_SHADER_BIT,
        Compute = GL_COMPUTE_SHADER_BIT,
        TessControl = GL_TESS_CONTROL_SHADER_BIT,
        TessEval = GL_TESS_EVALUATION_SHADER_BIT,
        Geometry = GL_GEOMETRY_SHADER_BIT
    };

public:
    Pipeline()
    {
        auto holder = std::make_shared<GLObjectHolder<Pipeline>>(1);
        _id = PipelineId(holder, 0);
    }

    Pipeline(const GLElementId<Pipeline>& id)
        : _id(id)
    {

    }

    inline void use(Type type, const Program& program)
    {
        glUseProgramStages(id(), (GLenum)type, program.id());
    }

    inline void unuse(Type type)
    {
        glUseProgramStages(id(), (GLenum)type, 0);
    }

    inline void bind()
    {
        glBindProgramPipeline(id());
    }

    inline GLuint id() const
    {
        return _id.id();
    }

private:
    using PipelineId = GLElementId<Pipeline>;
    PipelineId _id;

};



template<>
struct GLObjectHolder<Pipeline>
{
public:
    GLObjectHolder(size_t size)
    {
        GLsizei arr_size = size;

        _ids.resize(size, 0);
        glCreateProgramPipelines(arr_size, _ids.data());
    }

    ~GLObjectHolder()
    {
        if (_ids.size() == 0)
            return;

        glDeleteProgramPipelines(_ids.size(), _ids.data());
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

#pragma endregion

}
}

