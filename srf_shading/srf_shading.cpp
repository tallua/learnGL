
#include <srf/srf.h>

#include "SDL2/SDL.h"

#include "phong_renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>






class ShadingApp : public srf::system::Application
{
public:

    virtual void onInit()
    {
        _window = std::make_shared<GLWindow>();

        _window->setTitle("Shading Test");

        _window->makeCurrent();
        if (auto context = _window->context().lock())
        {
            context->clearColor(1, 1, 1, 1);
            context->vsync(true);
        }

        _renderer = std::make_shared<PhongRenderer>();

        auto obj3d = loadObjFile("3dobj/teapot.obj");
        auto graudmodel = std::make_shared<ModelBuffer>(*obj3d);

        auto cubeobj = loadObjFile("3dobj/cube.obj");
        auto cubemodel = std::make_shared<ModelBuffer>(*cubeobj);

        _cubes.push_back(PhongObject());
        _cubes[0].pos = glm::vec3(0.1f, 0.0f, 0.0f);
        _cubes[0].size = glm::vec3(0.2f, 0.2f, 0.2f) * 0.13f;
        _cubes[0].render_mode = 0;
        _cubes[0].color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        _cubes[0].model_buffer = graudmodel;

        //_cubes.push_back(Object());
        //_cubes[1].pos = glm::vec3(0.7f, 0.0f, 0.0f);
        //_cubes[1].size = glm::vec3(0.2f, 0.2f, 0.2f) * 0.13f;
        //_cubes[1].color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        //_cubes[1].model_buffer = flatmodel;

        _cubes.push_back(PhongObject());
        _cubes[1].pos = glm::vec3(-0.3f, 0.0f, 0.0f);
        _cubes[1].size = glm::vec3(0.2f, 0.2f, 0.2f) * 0.13f;
        _cubes[1].render_mode = 1;
        _cubes[1].color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        _cubes[1].model_buffer = graudmodel;

        _light_pos.size = glm::vec3(0.01f, 0.01f, 0.01f);
        _light_pos.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        _light_pos.model_buffer = cubemodel;

        _camera.pos = glm::vec3(0.3f, 0.2f, 0.4f) * 3.0f;
        _camera.forward = -glm::vec3(0.3f, 0.2f, 0.4f);
        _camera.up = glm::vec3(0, 1, 0);
        _camera.perspective(glm::pi<float>() / 2, (float)_window->size().x / _window->size().y, 0.1f, 10);

        _diff_tick = 0;
        _last_tick = SDL_GetTicks();
        _sum_tick = 0;
        _frame_cnt = 0;
    }

    virtual void onLoop()
    {
        // render
        _window->makeCurrent();
        if (auto context = _window->context().lock())
        {
            context->clear();

            _renderer->setCamera(_camera);
            _renderer->renderStart(context);

            std::for_each(_cubes.begin(), _cubes.end(),
                [this](const auto& target) { _renderer->render(target); });
            _renderer->render(_light_pos);

            _renderer->renderEnd(context);

            _window->swapWindow();
        }

        // update
        static float angle = 1.15f;
        static float height = 0.6f;
        static float dist = 0.8f;

        angle += 1.0f / 60;

        glm::vec3 pos(dist * cos(angle), height, dist * sin(angle));

        _light_pos.pos = pos;
        _renderer->setLight(-pos);
        //_cubes[1].pos += glm::vec3(0.01f, 0, 0.0f);

        // framecheck
        auto new_tick = SDL_GetTicks();
        _diff_tick = new_tick - _last_tick;
        _sum_tick += _diff_tick;
        _last_tick = new_tick;
        _frame_cnt += 1;
        if (_sum_tick >= 1000)
        {
            float fps = 1000.0f * _frame_cnt / _sum_tick;
            std::cout << "fps : " << fps << std::endl;

            _sum_tick = 0;
            _frame_cnt = 0;
        }
    }


    virtual void onExit()
    {
        _window.reset();
    }

private:

    // window & renderer
    sptr<GLWindow> _window;
    sptr<PhongRenderer> _renderer;

    // objects
    Camera _camera;
    std::vector<PhongObject> _cubes;
    PhongObject _light_pos;

    // framecheck
    int _diff_tick;
    int _last_tick;
    int _sum_tick;
    int _frame_cnt;
};




int main(int argc, char** argv)
{
    ShadingApp app;

    app.run();

    return 0;
}

