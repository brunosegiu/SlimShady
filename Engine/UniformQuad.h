#pragma once

#include <gl/glew.h>

#include "ShaderProgram.h"

class UniformQuad {
	public:
		GLuint id;
		ShaderProgram* filter;
		GLuint quad_vertexbuffer;
		UniformQuad();
		void draw();
};