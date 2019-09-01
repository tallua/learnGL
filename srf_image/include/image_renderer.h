#pragma once

#include "srf/srf.h"

using namespace srf::system;
using namespace srf::math;
using namespace srf::util;
using namespace srf::gl;

struct ImageTarget
{
    ImageTarget(std::string path)
    {
        render_mode = GL_TRIANGLE_FAN;

        pos[0] = { -1.0, -1.0};
        pos[1] = { -0.5, 0.5 };
        pos[2] = { 0.5, -0.5};

        color[0] = { 1.0, 0.0, 0.0, 1.0};
        color[1] = { 0.0, 1.0, 0.0, 1.0 };
        color[2] = { 0.0, 0.0, 1.0, 1.0 };

        uv[0] = { 0, 0};
        uv[1] = { 0, 0 };
        uv[2] = { 0, 0 };
    }

    GLenum render_mode = GL_TRIANGLE_FAN;
    glm::vec2 pos[3];
    glm::vec4 color[3];
    glm::vec2 uv[3];
};

class ImageRenderer :
    public srf::system::Renderer<GLContext, ImageTarget>,
    public srf::system::Renderer<GLContext, std::vector<ImageTarget>>
{
public:
    ImageRenderer();


    virtual void renderStart(wptr<GLContext> context) override;
    virtual void renderEnd(wptr<GLContext> context) override;

    virtual void render(const ImageTarget& target) override;
    virtual void render(const std::vector<ImageTarget>& targets) override;


private:
    //Uniform _u_texture;
    //Uniform _u_texture_valid;
    
    enum vao_map
    {
        pos,
        color
    };

    Program _program;
    VertexArray _vao;

    std::vector<Buffer> _buffers;
};