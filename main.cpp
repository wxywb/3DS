#include <graphics/FBO/experi_fbo.h>
#include <graphics/FBO/vanilla_fbo.h>

#include <graphics/graphics.h>

#include <chrono>
#include <exception>
#include <map>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#ifdef _WIN32
#include <windows.h>
#endif
#include <chrono>
using namespace std;

std::map<std::string, int> gbook;

template <typename T>
void global_store(string str, T &item) {
  gbook[str] = int(&item);
}

template <typename T>
T *global_load(string str) {
  return (T *)gbook[str];
}

std::chrono::steady_clock::time_point global_timestart;

int main(int argc, char *argv) {
  Magick::InitializeMagick(NULL);
  auto prender = graphics::init_graphics_executor("pachuri", 800, 600);
  graphics::ShaderManger vanilla_shader;
  prender->set_camera(
      new graphics::Camera(vmath::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.200));
  vanilla_shader.setVSFile("./resources/shaders/vanilla/vanilla.vs");
  vanilla_shader.setFSFile("./resources/shaders/vanilla/vanilla.fs");
  vanilla_shader.setup();
  vanilla_shader.use();

  vanilla_shader.send1i("diffuse_texture", COLOR_TEXTURE_UNIT_INDEX);
  vanilla_shader.send1i("normal_texture", NORMAL_TEXTURE_UNIT_INDEX);
  // graphics::Model
  // canvas(std::string("D:/proj/ogldev-source/Content/quad.obj"));
  graphics::Model canvas(std::string("./Content/tex/nanosuit2.obj"));
  // graphics::Model canvas(std::string("D:/Program Files/English PmxEditor
  // x64/models/Konpaku Youmu (Short Skirt)/Konpaku Youmu 2.00 (little
  // skirt).obj"));
  // graphics::Model canvas(
  //  std::string("./Content/boblampclean.md5mesh"));
  try {
    canvas.Init();
  } catch (std::exception &e) {
    cout << e.what() << endl;
    system("pause");
    abort();
  }
  gbook["render"] = int(&canvas);

  graphics::VanillaFramebufferobject fbo;
  fbo.initialize(600, 600);
  gbook["fbo"] = int(&fbo);

  cv::Mat cv_canvas = cv::Mat::zeros(600, 600, CV_8UC3);
  cv_canvas.at<cv::Vec3b>(10, 10) = cv::Vec3b(0, 0, 255);  // BGR
  gbook["cv_canvas"] = int(&cv_canvas);

  auto ptrCamera = prender->get_camera();
  vmath::mat4 pmat = vmath::perspective(120, 1, 0.2, 300);
  ptrCamera->set_instirc_property(60, 1, 0.2, 300);

  global_store("vanilla_shader", vanilla_shader);
  global_store("ptrCamera", *ptrCamera.get());
  global_timestart = std::chrono::high_resolution_clock::now();
  prender->render([]() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    auto start = std::chrono::high_resolution_clock::now();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto shader = global_load<graphics::ShaderManger>("vanilla_shader");
    auto ptrCamera = global_load<graphics::Camera>("ptrCamera");

    shader->send4fv("MVP", false, ptrCamera->pipeline());
    graphics::Model *pcanvas = (graphics::Model *)gbook["render"];
    // pcanvas->Render();
    cv::Mat *cv_canvas = (cv::Mat *)gbook["cv_canvas"];
    graphics::VanillaFramebufferobject *pfbo =
        (graphics::VanillaFramebufferobject *)gbook["fbo"];
    //	pfbo->use();
    pcanvas->Render();
    // pfbo->detach();
    vector<aiMatrix4x4> transforms;

    auto global_time_pass =
        std::chrono::high_resolution_clock::now() - global_timestart;
    float seconds_time_pass = (float)global_time_pass.count() / 1000;
    // pcanvas->BoneTransform(seconds_time_pass, transforms);

    cout << global_time_pass.count() << endl;
    Sleep(10);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_interval = end - start;
    //	cout << 1 / time_interval.count() << endl;
  });

  prender->run();
  prender->~Executor();
  return 0;
}
