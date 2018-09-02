#include <graphics/FBO/experi_fbo.h>

namespace graphics {


void ExperimentFramebufferobject::initialize(int width, int height) {
	Framebufferobject::initialize(width, height);
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	render_object = generateAttachmentTexture(GL_FALSE, GL_FALSE, width, height);
	render_object_red = generateAttachmentTexture(GL_FALSE, GL_FALSE, width, height);

	depth_object = generateAttachmentTexture(GL_TRUE, GL_FALSE, width, height);
	stencil_object = generateAttachmentTexture(GL_FALSE, GL_TRUE, width, height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		render_object, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 1, GL_TEXTURE_2D,
		render_object_red, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, depth_object);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
		GL_TEXTURE_2D, stencil_object);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	
	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	
	glDrawBuffers(2, attachments);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



}



