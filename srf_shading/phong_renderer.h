#pragma once

#include <srf/srf.h>
#include <srf/gl/gl.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace srf::util;
using namespace srf::gl;

inline std::string to_string(const glm::mat4& mat)
{
    std::stringstream iss;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            iss << std::to_string(mat[i][j]) << " ";
        }

        iss << std::endl;
    }

    return iss.str();
}

inline std::string to_string(const glm::vec3& vec)
{
    std::stringstream iss;
    for (int i = 0; i < 3; ++i)
    {
        iss << std::to_string(vec[i]) << " ";
    }

    return iss.str();
}

inline std::string to_string(const glm::vec4& vec)
{
    std::stringstream iss;
    for (int i = 0; i < 4; ++i)
    {
        iss << std::to_string(vec[i]) << " ";
    }

    return iss.str();
}

struct Camera
{
    glm::vec3 pos;
    glm::vec3 forward;
    glm::vec3 up;

    enum class Mode
    {
        Orthogonal,
        Perspective
    };

    Mode mode;
    float fovx;
    float aspect;
    float near, far;

    void perspective(float fovx, float aspect, float near, float far)
    {
        this->mode = Mode::Perspective;
        this->fovx = fovx;
        this->aspect = aspect;
        this->near = near;
        this->far = far;
    }

    void ortho()
    {

    }

    glm::mat4 viewMat() const
    {
        //glm::vec3 f = glm::normalize(forward);
        //glm::vec3 r = glm::normalize(glm::cross(f, up));
        //glm::vec3 u = glm::cross(r, f);
        //
        //
        //auto rotate = glm::mat4{
        //    { r.x, u.x, -f.x, 0 },
        //    { r.y, u.y, -f.y, 0 },
        //    { r.z, u.z, -f.z, 0 },
        //    {    0,    0,    0, 1 },
        //};
        //
        //auto translate = glm::mat4{
        //    { 1, 0, 0, 0},
        //    { 0, 1, 0, 0},
        //    { 0, 0, 1, 0},
        //    { -pos.x, -pos.y, -pos.z, 1}
        //};
        //
        //mat = rotate * translate;

        auto mat = glm::lookAtRH(pos, pos + forward, up);

        return mat;
    }

    glm::mat4 projMat() const
    {
        glm::mat4 result = glm::identity<glm::mat4>();
        switch (mode)
        {
        case Camera::Mode::Orthogonal:
            result = glm::mat4
            {
                {},
                {},
                {},
                {}
            };
            break;
        case Camera::Mode::Perspective:
            //result = glm::mat4
            //{
            //    { 1 / (glm::tan(fovx / 2)), 0, 0, 0 },
            //    { 0, aspect / (glm::tan(fovx / 2)), 0, 0},
            //    { 0, 0, -(far) / (far - near), -(far * near) / (far - near)},
            //    { 0, 0, -1, 0}
            //};
            result = glm::perspective(fovx / aspect, aspect, near, far);
            break;
        }

        return result;
    }
};

struct Res3DObject
{
    size_t vertex_count;

    bool use_uv;
    bool use_norm;

    std::vector<glm::vec3> pos;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec3> norm;

    //texture info
};


inline std::shared_ptr<Res3DObject> loadObjFile(const std::string& path)
{
    std::cout << "try loading : " << path << std::endl;
    std::ifstream ifs(path);
    std::vector<glm::vec3> v;
    std::vector<glm::vec3> vt;
    std::vector<glm::vec3> vn;

    if (!ifs.is_open())
        return std::shared_ptr<Res3DObject>(nullptr);

    auto result = std::make_shared<Res3DObject>();
    enum class ParseMode {
        None,
        V,
        V_VT,
        V_VT_VN,
        V__VN
    };
    ParseMode mode = ParseMode::None;

    while (!ifs.eof())
    {
        std::string line;
        std::getline(ifs, line);
        std::stringstream ss(line);

        std::string tag;
        std::string val1, val2, val3;
        ss >> tag >> val1 >> val2 >> val3;

        if (tag.compare("v") == 0)
        {
            v.push_back({ std::stof(val1), std::stof(val2), std::stof(val3) });
        }
        else if (tag.compare("vn") == 0)
        {
            vn.push_back({ std::stof(val1), std::stof(val2), std::stof(val3) });
        }
        else if (tag.compare("vt") == 0)
        {
            vt.push_back({ std::stof(val1), std::stof(val2), std::stof(val3) });
        }
        else if (tag.compare("f") == 0)
        {
            // set mode
            if (mode == ParseMode::None)
            {
                int cnt = std::count(val1.begin(), val1.end(), '/');

                switch (cnt)
                {
                case 0:
                    mode = ParseMode::V;
                    result->use_uv = false;
                    result->use_norm = false;
                    break;
                case 1:
                    mode = ParseMode::V_VT;
                    result->use_uv = true;
                    result->use_norm = false;
                    break;
                case 2:
                    int first_sep = val1.find_first_of('/');
                    int second_sep = val1.find_first_of('/', first_sep + 1);
                    if (first_sep + 1 == second_sep)
                    {
                        mode = ParseMode::V__VN;
                        result->use_uv = false;
                        result->use_norm = true;
                    }
                    else
                    {
                        mode = ParseMode::V_VT_VN;
                        result->use_uv = true;
                        result->use_norm = true;
                    }
                    break;
                }
            } // set mode end

            auto parse_v = [&](std::string& val) -> bool {
                size_t v_indx = std::stoi(val);
                if (v_indx > v.size())
                    return false;
                result->pos.push_back(v[v_indx - 1]);

                return true;
            };

            auto parse_v_vt = [&](std::string& val) -> bool {
                int pos = val.find_first_of('/');
                size_t v_indx = std::stoi(val.substr(0, pos));
                if (v_indx > v.size())
                    return false;
                result->pos.push_back(v[v_indx - 1]);

                int pos2 = val.size();
                size_t vt_indx = std::stoi(val.substr(pos + 1, pos2));
                if (vt_indx > vt.size())
                    return false;
                result->uv.push_back(vt[vt_indx - 1]);

                return true;
            };

            auto parse_v_vt_vn = [&](std::string& val) -> bool {
                int pos = val.find_first_of('/');
                size_t v_indx = std::stoi(val.substr(0, pos));
                if (v_indx > v.size())
                    return false;
                result->pos.push_back(v[v_indx - 1]);

                int pos2 = val.find_first_of('/', pos + 1);
                size_t vt_indx = std::stoi(val.substr(pos + 1, pos2));
                if (vt_indx > vt.size())
                    return false;
                result->uv.push_back(vt[vt_indx - 1]);

                int pos3 = val.size();
                size_t vn_indx = std::stoi(val.substr(pos2 + 1, pos3));
                if (vn_indx > vn.size())
                    return false;
                result->norm.push_back(vn[vn_indx - 1]);

                return true;
            };

            auto parse_v__vn = [&](std::string& val) -> bool {
                int pos = val.find_first_of('/');
                size_t v_indx = std::stoi(val.substr(0, pos));
                if (v_indx > v.size())
                    return false;
                result->pos.push_back(v[v_indx - 1]);

                int pos2 = val.find_first_of('/', pos + 1);

                int pos3 = val.size();
                size_t vn_indx = std::stoi(val.substr(pos2 + 1, pos3));
                if (vn_indx > vn.size())
                    return false;
                result->norm.push_back(vn[vn_indx - 1]);

                return true;
            };

            switch (mode)
            {
            case ParseMode::None:
                std::cout << "failed to set parse mode on line : " << std::endl;
                std::cout << "    " << line << std::endl;
                break;

            case ParseMode::V:
                if (!parse_v(val1)) return nullptr;
                if (!parse_v(val2)) return nullptr;
                if (!parse_v(val3)) return nullptr;

                result->vertex_count += 3;

                break;

            case ParseMode::V_VT:
                if (!parse_v_vt(val1)) return nullptr;
                if (!parse_v_vt(val2)) return nullptr;
                if (!parse_v_vt(val3)) return nullptr;

                result->vertex_count += 3;

                break;

            case ParseMode::V_VT_VN:
                if (!parse_v_vt_vn(val1)) return nullptr;
                if (!parse_v_vt_vn(val2)) return nullptr;
                if (!parse_v_vt_vn(val3)) return nullptr;

                result->vertex_count += 3;

                break;

            case ParseMode::V__VN:
                if (!parse_v__vn(val1)) return nullptr;
                if (!parse_v__vn(val2)) return nullptr;
                if (!parse_v__vn(val3)) return nullptr;

                result->vertex_count += 3;

                break;
            }

        }
    }

    ifs.close();

    return result;
}

struct ModelBuffer
{
    ModelBuffer(const Res3DObject& obj)
    {
        int cnt = 1 + (obj.use_uv ? 1 : 0) + (obj.use_norm ? 1 : 0);

        buffer = Buffer::Create(cnt);

        int next_index = 0;

        pos_index = next_index;
        next_index++;
        buffer[pos_index].data(obj.pos, Buffer::Usage::StaticDraw);

        if (obj.use_uv)
        {
            uv_index = next_index;
            next_index++;
            buffer[uv_index].data(obj.uv, Buffer::Usage::StaticDraw);
        }
        else
        {
            uv_index = -1;
        }

        if (obj.use_norm)
        {
            norm_index = next_index;
            next_index++;
            buffer[norm_index].data(obj.norm, Buffer::Usage::StaticDraw);
        }
        else
        {
            norm_index = -1;
        }

        vertex_count = obj.vertex_count;
    }

    int vertex_count;

    int pos_index;
    int uv_index;
    int norm_index;

    std::vector<Buffer> buffer;

    Buffer posBuffer() const
    {
        return buffer[pos_index];
    }
    Buffer uvBuffer() const
    {
        return buffer[uv_index];
    }
    Buffer normBuffer() const
    {
        return buffer[norm_index];
    }
};

struct PhongObject
{
    glm::vec3 size;
    glm::vec3 pos;
    glm::quat rot;

    glm::vec4 color;

    int render_mode;

    std::shared_ptr<ModelBuffer> model_buffer;
};



class PhongRenderer
    : srf::system::Renderer<GLContext, PhongObject>
{
public:
    PhongRenderer()
    {
        Shader vs;
        vs.create(Shader::Type::Vertex);
        vs.source(obj_vs);
        vs.compile();
        if (!vs.compileStatus())
        {
            std::cout << "VS compile error : " << std::endl;
            std::cout << vs.infolog() << std::endl;
        }

        Shader fs;
        fs.create(Shader::Type::Fragment);
        fs.source(obj_fs);
        fs.compile();
        if (!fs.compileStatus())
        {
            std::cout << "FS compile error : " << std::endl;
            std::cout << fs.infolog() << std::endl;
        }

        if (vs.compileStatus() && fs.compileStatus())
        {
            _program.create();
            _program.attach(vs);
            _program.attach(fs);

            _program.link();

            if (!_program.linkStatus())
            {
                std::cout << "Link Error " << std::endl;
                std::cout << _program.infoLog() << std::endl;
            }

            u_model = Uniform(_program, "u_model");
            u_view = Uniform(_program, "u_view");
            u_proj = Uniform(_program, "u_proj");

            u_light_dir = Uniform(_program, "u_light_dir");
            u_color = Uniform(_program, "u_color");

            auto i_pos = Attribute(_program, "i_pos");
            auto i_nor = Attribute(_program, "i_nor");

            _vertexarr.format<glm::vec3>(i_pos, GL_FALSE, 0);
            _vertexarr.map(i_pos, 0);

            _vertexarr.format<glm::vec3>(i_nor, GL_FALSE, 0);
            _vertexarr.map(i_nor, 1);
        }
    }

    virtual void renderStart(wptr<GLContext> context)
    {

        if (!_program.linkStatus())
        {
            return;
        }

        if (auto gl = context.lock())
        {
            gl->depthTest(true);
        }

        _program.use();

        _vertexarr.enable(0);
        _vertexarr.enable(1);
    }

    virtual void renderEnd(wptr<GLContext> context)
    {
        _vertexarr.disable(1);
        _vertexarr.disable(0);
        Program::UnuseAll();
    }



    virtual void render(const PhongObject& target)
    {
        auto model = glm::identity<glm::mat4>();
        auto size = glm::mat4{
            { target.size.x, 0, 0, 0 },
            { 0, target.size.y, 0, 0 },
            { 0, 0, target.size.z, 0 },
            { 0, 0, 0,             1 }
        };
        auto rot = glm::toMat4(target.rot);
        auto pos = glm::mat4{
            { 1, 0, 0, 0},
            { 0, 1, 0, 0},
            { 0, 0, 1, 0},
            { target.pos.x, target.pos.y, target.pos.z, 1}
        };

        model = pos * rot * size * model;

        u_model.set(model);
        u_color.set(target.color);

        _vertexarr.vertexBuffer(0, target.model_buffer->posBuffer(), 0, sizeof(glm::vec3));
        _vertexarr.vertexBuffer(1, target.model_buffer->normBuffer(), 0, sizeof(glm::vec3));

        _vertexarr.bind();
        glDrawArrays(GL_TRIANGLES, 0, target.model_buffer->vertex_count);
    }

    virtual void setCamera(const Camera& camera)
    {
        u_view.set(camera.viewMat());
        u_proj.set(camera.projMat());
    }

    virtual void setLight(glm::vec3 dir)
    {
        u_light_dir.set(dir);
    }


private:
    Uniform u_model;
    Uniform u_view;
    Uniform u_proj;

    Uniform u_light_dir;
    Uniform u_color;

    VertexArray _vertexarr;
    Program _program;

    const char* obj_vs = ""
        "#version 460                                                       \n"
        "                                                                   \n"
        "                                                                   \n"
        "uniform mat4 u_model;                                              \n"
        "uniform mat4 u_view;                                               \n"
        "uniform mat4 u_proj;                                               \n"
        "                                                                   \n"
        "// light & shading                                                 \n"
        "uniform vec3 u_light_dir;                                          \n"
        "                                                                   \n"
        "                                                                   \n"
        "uniform vec4 u_color;                                              \n"
        "                                                                   \n"
        "// pos                                                             \n"
        "layout(location = 0) in vec3 i_pos;                                \n"
        "layout(location = 1) in vec3 i_nor;                                \n"
        "                                                                   \n"
        "                                                                   \n"
        "out vec4 v_color;                                                  \n"
        "out vec3 v_cs_nor;                                                 \n"
        "out vec4 v_cs_pos;                                                 \n"
        "out vec3 v_cs_light_dir;                                           \n"
        "                                                                   \n"
        "void main(void)                                                    \n"
        "{                                                                  \n"
        "    mat4 mv = u_view * u_model;                                    \n"
        "                                                                   \n"
        "    vec3 dir = u_light_dir;                                        \n"
        "    vec3 cs_nor = normalize((transpose(inverse(mv))                \n"
        "        * vec4(i_nor, 0.0f)).xyz);                                 \n"
        "                                                                   \n"
        "    vec3 cs_light_dir = normalize((transpose(inverse(u_view))      \n"
        "        * vec4(u_light_dir, 0.0f)).xyz);                           \n"
        "                                                                   \n"
        "    vec4 cs_pos = mv * vec4(i_pos, 1.0f);                          \n"
        "    vec4 ss_pos = u_proj * cs_pos;                                 \n"
        "                                                                   \n"
        "    v_cs_light_dir = cs_light_dir;                                 \n"
        "    v_cs_nor = cs_nor;                                             \n"
        "    v_cs_pos = cs_pos;                                             \n"
        "    v_color = u_color;                                             \n"
        "    gl_Position = ss_pos;                                          \n"
        "                                                                   \n"
        "}                                                                  \n"
        "                                                                   \n";

    const char* obj_fs = ""
        "#version 460                                                       \n"
        "                                                                   \n"
        "uniform vec3 u_light_dir;                                          \n"
        "                                                                   \n"
        "in vec4 v_color;                                                   \n"
        "in vec3 v_cs_nor;                                                  \n"
        "in vec4 v_cs_pos;                                                  \n"
        "in vec3 v_cs_light_dir;                                            \n"
        "                                                                   \n"
        "                                                                   \n"
        "layout(location = 0) out vec4 f_color;                             \n"
        "                                                                   \n"
        "                                                                   \n"
        "void main(void)                                                    \n"
        "{                                                                  \n"
        "    vec3 cs_nor = normalize(v_cs_nor);                             \n"
        "    float diff_factor = dot(cs_nor, -u_light_dir);                 \n"
        "    vec4 diff_color = clamp(diff_factor * v_color, 0.0f, 1.0f);    \n"
        "                                                                   \n"
        "                                                                   \n"
        "    vec3 reflected = reflect(-v_cs_light_dir, v_cs_nor);           \n"
        "    float spec_factor = dot(v_cs_pos.xyz, reflected);              \n"
        "    spec_factor = pow(clamp(spec_factor, 0.0f, 1.0f), 4);          \n"
        "    vec4 spec_color = spec_factor * vec4(1, 0, 0, 1);              \n"
        "                                                                   \n"
        "    f_color = diff_color + spec_color;                             \n"
        "}                                                                  \n"
        "                                                                   \n"
        "                                                                   \n";
};


