#include <graphics/FBO/framebufferobject.h>

namespace graphics {
void Framebufferobject::initialize(size_t width, size_t height) {
#ifdef _DEBUG
	std::cout << "framebuffer has initialized!" << std::endl;
#endif
	this->width = width;
	this->height = height;

}


void Framebufferobject::prepare() {

}

void Framebufferobject::use() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}


void Framebufferobject::detach() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}

