#pragma once

#include <string>

#include "UniformQuad.h"
#include "ShaderProgram.h"
#include "FBO.h"

using namespace std;

class Filter {
	public:
		static UniformQuad* quad;
		static FBO* fbo;
		ShaderProgram* shader;
		Filter(string fragLoc, unsigned int width, unsigned int height);
		void bind();
};