
#include<graphics/common.h>
#include <Magick++/Magick++.h>

#ifndef SHUKI_TEXTURE
#define SHUKI_TEXTURE

namespace graphics {

class Texture
{
public:
	Texture(GLenum TextureTarget, const std::string& FileName);

	bool Load();

	void Bind(GLenum TextureUnit);

private:
	std::string 		m_fileName;
	GLenum 				m_textureTarget;
	GLuint 				m_textureObj;
	Magick::Image		m_image;
	Magick::Blob 		m_blob;
};

}
#endif