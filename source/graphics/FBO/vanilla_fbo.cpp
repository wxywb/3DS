#include <graphics/FBO/vanilla_fbo.h>

namespace graphics {

void VanillaFramebufferobject::initialize(int width, int height) {
  Framebufferobject::initialize(width, height);
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  render_object = generateAttachmentTexture(GL_FALSE, GL_FALSE, width, height);
  depth_object = generateAttachmentTexture(GL_TRUE, GL_FALSE, width, height);
  stencil_object = generateAttachmentTexture(GL_FALSE, GL_TRUE, width, height);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         render_object, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                            depth_object);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
                            GL_TEXTURE_2D, stencil_object);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"
              << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}  // namespace graphics
