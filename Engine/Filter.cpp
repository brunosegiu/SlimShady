#include "Filter.h"

UniformQuad* Filter::quad = NULL;
FBO* Filter::fbo = NULL;

Filter::Filter(string fragPath, unsigned int width, unsigned int height) {
	if (quad == NULL) {
		quad = new UniformQuad();
	}
	if (fbo == NULL) {
		fbo = new FBO(width, height);
	}
	this->shader = new ShaderProgram("assets/shaders/pass.vert", fragPath);
}

void Filter::bind() {
	this->shader->bind();
}