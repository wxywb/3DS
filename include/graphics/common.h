

#include <GL/glew.h>
#include <GL/GL.h>
#include <graphics/helper.h>
#include <graphics/vmath.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
//
#ifndef OGL_COMMON_H
#define OGL_COMMON_H

#define COLOR_TEXTURE_UNIT GL_TEXTURE0
#define COLOR_TEXTURE_UNIT_INDEX 0
#define SHADOW_TEXTURE_UNIT GL_TEXTURE1
#define SHADOW_TEXTURE_UNIT_INDEX 1
#define NORMAL_TEXTURE_UNIT GL_TEXTURE2
#define NORMAL_TEXTURE_UNIT_INDEX 2
#define RANDOM_TEXTURE_UNIT GL_TEXTURE3
#define RANDOM_TEXTURE_UNIT_INDEX 3
#define DISPLACEMENT_TEXTURE_UNIT GL_TEXTURE4
#define DISPLACEMENT_TEXTURE_UNIT_INDEX 4
#define MOTION_TEXTURE_UNIT GL_TEXTURE5
#define MOTION_TEXTURE_UNIT_INDEX 5
#define CASCACDE_SHADOW_TEXTURE_UNIT0 SHADOW_TEXTURE_UNIT
#define CASCACDE_SHADOW_TEXTURE_UNIT0_INDEX SHADOW_TEXTURE_UNIT_INDEX
#define CASCACDE_SHADOW_TEXTURE_UNIT1 GL_TEXTURE6
#define CASCACDE_SHADOW_TEXTURE_UNIT1_INDEX 6
#define CASCACDE_SHADOW_TEXTURE_UNIT2 GL_TEXTURE7
#define CASCACDE_SHADOW_TEXTURE_UNIT2_INDEX 7

#define LOG(m) LOG_MESS(m)
#define GLCheckError() (glGetError() == GL_NO_ERROR)
#define SAFE_DELETE(p) \
  if (p) {             \
    delete p;          \
    p = NULL;          \
  }
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a) / sizeof(a[0]))

void static LOG_MESS(char* message) { std::cout << message << std::endl; }

bool static ReadFile(std::string filename, std::string& content) {
  std::ifstream in(filename);
  std::string buffer;
  if (!in.is_open()) {
    LOG("file doesn' exist");
    return false;
  }
  while (!in.eof()) {
    getline(in, buffer);
    content += buffer;
    content += '\n';
  }
  return true;
}

#endif