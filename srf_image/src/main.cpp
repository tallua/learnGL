
#include "srf/srf.h"

#include <iostream>

#include <vector>
#include <algorithm>

#include "../include/image_renderer.h"

using namespace srf::system;

class MyApp : public srf::system::Application
{
protected:
    virtual void onInit() override
    {
        _images.push_back(ImageTarget("resource/img1.jpg"));
        //_images.push_back(ImageTarget("resource/img2.jpg"));
        //_images.push_back(ImageTarget("resource/img3.jpg"));
        //_images.push_back(ImageTarget("resource/img4.jpg"));


        _window = std::make_shared<GLWindow>();

        _window->makeCurrent();
        if (auto context = _window->context().lock())
        {
            context->clearColor(1, 1, 1, 1);
        }

        _renderer = std::make_shared<ImageRenderer>();
    }

    virtual void onLoop() override
    {

        // update


        // render
        _window->makeCurrent();
        if (auto context = _window->context().lock())
        {
            context->clear(); 

            _renderer->renderStart(context);

            std::for_each(_images.begin(), _images.end(),
                [this](const auto & target) { _renderer->render(target); });

            _renderer->renderEnd(context);

            _window->swapWindow();
        }
    }

    virtual void onStop() override
    {
        _renderer = nullptr;
        _window = nullptr;
    }


private:
    sptr<GLWindow> _window;
    
    sptr<ImageRenderer> _renderer;
    std::vector<ImageTarget> _images;
};

int main(int argc, char** argv)
{
    MyApp app;

    app.run();

    return 0;
}