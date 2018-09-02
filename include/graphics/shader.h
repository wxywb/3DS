
#include "common.h"
//#include"general_utils.h"

#ifndef SHADER_HPP
#define SHADER_HPP

namespace graphics {

static GLchar ErrorLog[1024] = { 0 };

class Shader {
public:
	Shader() {}

	bool initialize_shaders();

	GLenum mshader_type;
	GLuint mshader;
	::std::string filename;
};

class ShaderManger {
public:
	ShaderManger() {}

	void init_program();

	ShaderManger &setVSFile(std::string vsfilename_);

	ShaderManger &setFSFile(std::string fsfilename_);

	ShaderManger &setDir(std::string dir_name_);

	~ShaderManger() {};

	ShaderManger &initialize();

	void attach_shaders();

	void use();

	GLuint getprogram();

	GLuint get_uniform_loc(std::string str);

	void send1i(std::string name, GLuint value);

	void send1f(std::string name, GLfloat value);

	void send4fv(std::string name, bool transpose, const GLfloat* value);

	void setup();
private:
	std::string dir;
	const char *vsfilename;
	const char *fsfilename;
	std::vector<Shader> mshaders;
	GLuint mshaderprogram;
};


}  // namespace graphics

#endif
