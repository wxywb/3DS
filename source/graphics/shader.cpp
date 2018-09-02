#include <graphics/Shader.h>


namespace graphics {

bool Shader::initialize_shaders() {
	mshader = glCreateShader(this->mshader_type);
	std::string file_content;
	const GLchar* p[1];
	GLint lengths[1];
	if (ReadFile(filename, file_content)) {
		p[0] = file_content.c_str();
		lengths[0] = strlen(file_content.c_str());
		glShaderSource(mshader, 1, p, lengths);
		glCompileShader(mshader);

		GLint success = 0;
		glGetShaderiv(mshader, GL_COMPILE_STATUS, &success);
		if (!success) {
			GLchar InfoLog[1024];
			glGetShaderInfoLog(mshader, 1024, NULL, InfoLog);
			fprintf(stderr, "Error compiling shader type %d: '%s'\n",
				this->mshader_type, InfoLog);
			exit(1);
		}
		return true;
	}
	else {
		return false;
	}
}

void ShaderManger::init_program() {
	mshaderprogram = glCreateProgram();
	if (mshaderprogram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}
}

ShaderManger& ShaderManger::setVSFile(std::string vsfilename_) {
	vsfilename = vsfilename_.c_str();
	Shader vs_shader;
	vs_shader.filename = vsfilename_;
	vs_shader.mshader_type = GL_VERTEX_SHADER;
	mshaders.push_back(vs_shader);
	return *this;
}

ShaderManger& ShaderManger::setFSFile(std::string fsfilename_) {
	fsfilename = fsfilename_.c_str();
	Shader fs_shader;
	fs_shader.filename = fsfilename_;
	fs_shader.mshader_type = GL_FRAGMENT_SHADER;
	mshaders.push_back(fs_shader);
	return *this;
}

ShaderManger& ShaderManger::setDir(std::string dir_name_) {
	fsfilename = dir_name_.c_str();
	return *this;
}

ShaderManger& ShaderManger::initialize() {
	for (auto& shader : mshaders) {
		shader.initialize_shaders();
	}
	return *this;
}

void ShaderManger::attach_shaders() {
	for (auto shader : mshaders) {
		glAttachShader(mshaderprogram, shader.mshader);
	}
	glLinkProgram(mshaderprogram);
	GLint Success;
	glGetProgramiv(mshaderprogram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(mshaderprogram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}
	glValidateProgram(mshaderprogram);
}

void ShaderManger::use() { glUseProgram(mshaderprogram); }

GLuint ShaderManger::getprogram() { return mshaderprogram; }

inline GLuint ShaderManger::get_uniform_loc(std::string str) {
	return glGetUniformLocation(mshaderprogram, str.c_str());
}

void ShaderManger::send1i(std::string name, GLuint value) {
	GLuint location = this->get_uniform_loc(name.c_str());
	glUniform1i(location, value);
}

void ShaderManger::send1f(std::string name, GLfloat value) {
	GLuint location = this->get_uniform_loc(name.c_str());
	glUniform1f(location, value);
}

void ShaderManger::send4fv(std::string name, bool transpose, const GLfloat* value) {
	GLuint location = this->get_uniform_loc(name.c_str());
	glUniformMatrix4fv(location, 1, transpose, value);
}

void ShaderManger::setup() {
	initialize();
	init_program();
	attach_shaders();
}

}  // namespace graphics
