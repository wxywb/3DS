#include <graphics/FBO/framebufferobject.h>
#include <graphics/common.h>

namespace graphics {

class ExperimentFramebufferobject : public Framebufferobject {
 public:
  void initialize(int width, int height);

 private:
  GLuint render_object;
  GLuint render_object_red;
  GLuint stencil_object;
  GLuint depth_object;
};
}  // namespace graphics