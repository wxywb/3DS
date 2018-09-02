#include<graphics/executor.h>
#include<graphics/callback.h>

namespace graphics {


Executor::Executor(std::string name, size_t width, size_t height) {

	if (!glfwInit()) {
#ifdef _DEBUG
		std::cout << "glfwInit() failed!" << std::endl;
#endif 
		exit(EXIT_FAILURE);
	}
	window = glfwCreateWindow(width, height, "Simple example", NULL, NULL);
	glfwMakeContextCurrent(window);
	Callback::Init();
	glfwSetKeyCallback(window, Callback::key_callback);
	glfwSetCursorPosCallback(window, Callback::cursor_position_callback);
	GLenum res = glewInit();
#ifdef _DEBUG
	if (res != GLEW_OK) {
		fprintf(stderr, "glew initialized failed: '%s'\n", glewGetErrorString(res));
		return;
	}
#endif 
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void Executor::set_camera(Camera * pCamera) {
	ptrcamera.reset();
	ptrcamera = std::shared_ptr<Camera>(pCamera);
	Callback::ptr_camera = ptrcamera;
}


void Executor::run() {
	if (!rending_delegate)
		return;
	while (!glfwWindowShouldClose(window))
	{
		this->rending_delegate();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

Executor::~Executor() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Executor::render(const std::function<void(void)> & task) {
	this->rending_delegate = task;
}

void Executor::init(const std::function<void(void)> & task) {
	task();
}

std::shared_ptr<Camera> Executor::get_camera() {
	return std::shared_ptr<Camera>(this->ptrcamera);
}


std::shared_ptr<Executor> init_graphics_executor(std::string name, size_t width, size_t height) {
	static std::shared_ptr<Executor> handle;
	if (handle == nullptr) {
		auto ptr = new Executor(name, width, height);
		handle = std::shared_ptr<Executor>(ptr);
	}
	return std::shared_ptr<Executor>(handle);
}


}

