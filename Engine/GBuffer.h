#pragma once

#include <GL/glew.h>

class GBuffer {
	public:
		unsigned int width, height;
		GLuint fbo;
		GLuint posTID, diffTID, normTID, textTID, depthTID;
	public:
		GBuffer(unsigned int width, unsigned int height);
		~GBuffer();
		void bindW();
		void bindR();
		void readPos();
		void readDiff();
		void readNorm();
		void readText();
};