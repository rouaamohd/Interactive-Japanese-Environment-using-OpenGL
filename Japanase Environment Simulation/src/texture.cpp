#include "texture.h"
// texture rendering
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h";

Texture::Texture():mTexture(0)
{
	
}
Texture::~Texture()
{
	glDeleteTextures(1, &mTexture);
}
bool Texture::loadTexture(const string& filename, bool generateMipMap) {

	GLint width, height, nrComponents;

	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	// set parameters and wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);

	// load texture from file
	unsigned char* texData = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

	GLenum format;
	if (nrComponents == 1)
		format = GL_RED;
	if (nrComponents == 3)
		format = GL_RGB;
	if (nrComponents == 4)
		format = GL_RGBA;

	if (!texData)
	{
		printf("Failing to load image: %s\n", filename);
	}
	if (generateMipMap)
		glGenerateMipmap(GL_TEXTURE_2D); // optional

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, texData);
	stbi_image_free(texData);
	return true;
}
void Texture::bind(GLuint texUnit)
{
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, mTexture);
}

//unbind
void Texture::unbind(GLuint texUnit)
{
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, 0); //unbind
}



