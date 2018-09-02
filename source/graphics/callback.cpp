#include <graphics/callback.h>
#include <map>
#include <functional>
#include <string>
namespace graphics {
using Event_ = std::function<void(void)>;

std::shared_ptr<Camera> Callback::ptr_camera = nullptr;

std::map<int, Event_> IO_event_manger;
void Callback::Init() {
	IO_event_manger[GLFW_KEY_UP] = []() {
		Callback::ptr_camera->move(ptr_camera->getfront());
		float x = ptr_camera->getPosition()[0];
		float y = ptr_camera->getPosition()[1];
		float z = ptr_camera->getPosition()[2];
		std::cout << x << ' ' << y << ' '<< z << std::endl;
		std::cout << "up has been invoked!" << std::endl;
	};
	IO_event_manger[GLFW_KEY_DOWN] = []() {
		vmath::vec3 direction = ptr_camera->getfront() * -1.0f;
		Callback::ptr_camera->move(direction);

		float x = ptr_camera->getPosition()[0];
		float y = ptr_camera->getPosition()[1];
		float z = ptr_camera->getPosition()[2];
		std::cout << x << ' ' << y << ' ' << z << std::endl;
		std::cout << "down has been invoked!" << std::endl;
	};
	IO_event_manger[GLFW_KEY_LEFT] = []() {
		vmath::vec3 direction = ptr_camera->getright();
		Callback::ptr_camera->move(direction* -1.0f);
		float x = ptr_camera->getPosition()[0];
		float y = ptr_camera->getPosition()[1];
		float z = ptr_camera->getPosition()[2];
		std::cout << x << ' ' << y << ' ' << z << std::endl;
		std::cout << "left has been invoked!" << std::endl;
	};
	IO_event_manger[GLFW_KEY_RIGHT] = []() {
		vmath::vec3 direction = ptr_camera->getright();
		Callback::ptr_camera->move(direction);
		float x = ptr_camera->getPosition()[0];
		float y = ptr_camera->getPosition()[1];
		float z = ptr_camera->getPosition()[2];
		std::cout << x << ' ' << y << ' ' << z << std::endl;
		std::cout << "right has been invoked!" << std::endl;
	};

	IO_event_manger[GLFW_KEY_W] = IO_event_manger[GLFW_KEY_UP];
	IO_event_manger[GLFW_KEY_S] = IO_event_manger[GLFW_KEY_DOWN];
	IO_event_manger[GLFW_KEY_A] = IO_event_manger[GLFW_KEY_LEFT];
	IO_event_manger[GLFW_KEY_D] = IO_event_manger[GLFW_KEY_RIGHT];

	IO_event_manger[GLFW_KEY_SPACE] = []() {
		vmath::vec3 direction = vmath::vec3(0.0f, 1.0f, 0.0f);
		Callback::ptr_camera->move(direction);
		std::cout << "right has been invoked!" << std::endl;
	};

	IO_event_manger[GLFW_KEY_LEFT_CONTROL] = []() {
		vmath::vec3 direction = vmath::vec3(0.0f, -1.0f, 0.0f);
		Callback::ptr_camera->move(direction);
		std::cout << "right has been invoked!" << std::endl;
	};
}
void Callback::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (IO_event_manger.find(key) == IO_event_manger.end()) {
		return;
	}
	else {
		IO_event_manger[key]();
	}
}

void Callback::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	Callback::ptr_camera->point(xpos, ypos);
	std::cout << xpos << " " << ypos << std::endl;

}


}