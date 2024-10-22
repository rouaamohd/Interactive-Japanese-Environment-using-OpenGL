#ifndef TEXTURE_H
#define TEXTURE_H

#include "GL/glew.h"
#include <string>
using namespace std;

class Texture
{
public:
	Texture();
	virtual ~Texture(); //virtual to enable others to add to your code

	bool loadTexture(const string &filename, bool generateMipMap);
	void bind(GLuint texUnit = 0);
	void unbind(GLuint texUnit);


private:
	GLuint mTexture;
};









#endif // !TEXTURE_H

