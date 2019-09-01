#include "../include/image_renderer.h"

#include <iostream>
#include <algorithm>


const char* vs_src = ""
"#version 440								    \n"
"											    \n"
"layout(location = 0) in vec2 i_pos_in;		    \n"
"layout(location = 1) in vec4 i_color_in;	    \n"
"											    \n"
"out vec4 v_color_out;	                        \n"
"out vec2 v_uv_out;		                        \n"
"											    \n"
"void main(void)								    \n"
"{											    \n"
"	v_color_out = i_color_in;					\n"
"	gl_Position = vec4(i_pos_in, 0.0f, 1.0f);	\n"
"}										    	\n";

const char* fs_src = ""
"#version 440											\n"
"														\n"
"														\n"
"in vec4 v_color_out;                   				\n"
"														\n"
"layout(location = 0) out vec4 f_color_out;				\n"
"														\n"
"void main(void)									    \n"
"{														\n"
"	vec4 text_color = vec4(1.0, 1.0, 1.0, 1.0);			\n"
"														\n"
"	f_color_out = text_color * v_color_out;				\n"
"}														\n";


ImageRenderer::ImageRenderer()
{
    Shader vs;
    Shader fs;
    vs.create(Shader::Type::Vertex);
    fs.create(Shader::Type::Fragment);

    vs.source(vs_src);
    vs.compile();
    bool vs_valid = vs.compileStatus();

    fs.source(fs_src);
    fs.compile();
    bool fs_valid = fs.compileStatus();

    if (vs_valid && fs_valid)
    {
        _program.create();
        _program.attach(vs);
        _program.attach(fs);

        _program.link();
        if (!_program.linkStatus())
        {
            std::cout << "Something wrong on linking program : " << std::endl;
            std::cout << _program.infoLog() << std::endl;
        }
    }
    else if (!vs_valid)
    {
        std::cout << "Something wrong on vs shader code" << std::endl;
        std::cout << vs.infolog() << std::endl;
    }
    else if (!fs_valid)
    {
        std::cout << "Something wrong on fs shader code" << std::endl;
        std::cout << fs.infolog() << std::endl;
    }

    // buffer
    _buffers = Buffer::Create(3);

    // attribute
    Attribute i_pos_in(_program, "i_pos_in");
    Attribute i_color_in(_program, "i_color_in");
    Attribute i_uv_in(_program, "i_uv_in");

    _vao.format(i_pos_in, 2, GL_FLOAT, GL_FALSE, 0);
    _vao.map(i_pos_in, vao_map::pos);
    _vao.vertexBuffer(vao_map::pos, _buffers[0], 0, sizeof(glm::vec2));

    _vao.format(i_color_in, 4, GL_FLOAT, GL_FALSE, 0);
    _vao.map(i_color_in, vao_map::color);
    _vao.vertexBuffer(vao_map::color, _buffers[1], 0, sizeof(glm::vec4));

    // uniform
    //_u_texture = Uniform(_program, "i_texture_in");
    //_u_texture_valid = Uniform(_program, "i_texture_valid");

    //_u_texture.set(0);
    //_u_texture_valid.set(0);

}

void ImageRenderer::renderStart(wptr<GLContext> context)
{
    if (!_program.linkStatus())
    {
        return;
    }

    if (auto gl = context.lock())
    {
        gl->depthTest(true);
    }
    else
    {
        // something went wrong
    }

    _program.use();

    _vao.enable(vao_map::pos);
    _vao.enable(vao_map::color);
    //_vao.enable(0, _i_uv_in);

}

void ImageRenderer::renderEnd(wptr<GLContext> context)
{
    _vao.disable(vao_map::pos);
    _vao.disable(vao_map::color);
    //_vao.disable(0, _i_uv_in);
    Program::UnuseAll();
}

void ImageRenderer::render(const ImageTarget& target)
{
    //_u_texture.set(0);
    //_u_texture_valid.set(0);

    _buffers[0].data(3 * sizeof(glm::vec2), (void*)glm::value_ptr(target.pos[0]), Buffer::Usage::StaticDraw);
    _buffers[1].data(3 * sizeof(glm::vec4), (void*)glm::value_ptr(target.color[0]), Buffer::Usage::StaticDraw);
    //_buffer.data(2, 3 * sizeof(glm::vec2), (void*)glm::value_ptr(target.uv[0]), Buffers::Usage::StaticDraw);

    _vao.bind();
    glDrawArrays(target.render_mode, 0, 3);
}

void ImageRenderer::render(const std::vector<ImageTarget>& targets)
{
    for (auto& target : targets)
    {
        render(target);
    }
}
