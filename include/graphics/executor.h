#include <graphics/camera.h>
#include <graphics/common.h>

#include <GLFW/glfw3.h>
#ifndef EXECUTOR_H
#define EXECUTOR_H

namespace graphics {

class Executor {
    friend std::shared_ptr<Executor> init_graphics_executor(std::string name, size_t width, size_t height);

public:
    void init(const std::function<void(void)>& task);

    void render(const std::function<void(void)>& task);

    void set_camera(Camera* pCamera);

    std::shared_ptr<Camera> get_camera();

    void run();

    ~Executor();

private:
    Executor(std::string name, size_t width, size_t height);
    std::string _name;
    size_t _width;
    size_t _height;
    GLFWwindow* window;
    std::function<void(void)> rending_delegate;
    std::shared_ptr<Camera> ptrcamera;
};

//std::shared_ptr<Executor> init_graphics_executor(std::string name, size_t width, size_t height);
}

#endif