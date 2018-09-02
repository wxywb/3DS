#pragma once
#include <graphics/common.h>
#include <graphics/camera.h>
#include <GLFW/glfw3.h>
#ifndef CALLBACK_H
#define CALLBACK_H

namespace graphics {
class Callback {
public:

	static void Init();

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

	static std::shared_ptr<Camera>  ptr_camera;
};

}

#endif