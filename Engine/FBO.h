#pragma once

#include <GL/glew.h>

class FBO {
public:
	GLuint id, textid, oldtext;
	GLuint depthrenderbuffer, textDB;
	unsigned int width, height;
	FBO(unsigned int width, unsigned int height);
	void bind();
};
