#pragma once

#include <string>
#include <GL/glew.h>

class CTexture
{
public:
	CTexture(const std::string fileName);
	CTexture();
	CTexture(unsigned int width, unsigned int height);

	GLuint getTextureData();

	~CTexture();

	void bind(unsigned int unit);
	void unBind();

private:

	GLuint texture;

};