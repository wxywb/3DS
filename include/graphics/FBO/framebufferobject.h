#include <graphics/common.h>
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
namespace graphics {
class Framebufferobject {
 public:
  virtual void initialize(size_t width, size_t height);

  virtual void prepare();

  virtual void use();

  virtual void detach();

 private:
 protected:
  GLuint fbo;
  size_t width;
  size_t height;
};
}  // namespace graphics

#endif