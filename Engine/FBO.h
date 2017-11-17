#pragma once

#include <GL/glew.h>

class FBO {
public:
	GLuint id, textid;
	GLuint depthrenderbuffer;
	unsigned int width, height;
	FBO(unsigned int width, unsigned int height);
	void bind();
};
