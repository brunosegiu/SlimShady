#include "GBuffer.h"

GBuffer::GBuffer(unsigned int width, unsigned int height) {
	// Generate FBO
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

	// Generate textures
	glGenTextures(1, &this->posTID);
	glGenTextures(1, &this->diffTID);
	glGenTextures(1, &this->normTID);
	glGenTextures(1, &this->textTID);
	glGenTextures(1, &this->depthTID);

	//Allocate memory for the textures
	
	glBindTexture(GL_TEXTURE_2D, this->posTID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, (void*)0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->posTID, 0);

	glBindTexture(GL_TEXTURE_2D, this->diffTID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, (void*)0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->diffTID, 0);

	glBindTexture(GL_TEXTURE_2D, this->normTID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, (void*)0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->normTID, 0);

	glBindTexture(GL_TEXTURE_2D, this->textTID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, (void*)0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, this->textTID, 0);

	glBindTexture(GL_TEXTURE_2D, this->depthTID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, (void*)0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthTID, 0);
	
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, drawBuffers);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

GBuffer::~GBuffer(){

}

void GBuffer::bindW(){
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

void GBuffer::bindR(){
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
}

void GBuffer::readPos() {
	glReadBuffer(GL_COLOR_ATTACHMENT0);
}

void GBuffer::readDiff() {
	glReadBuffer(GL_COLOR_ATTACHMENT1);
}

void GBuffer::readNorm() {
	glReadBuffer(GL_COLOR_ATTACHMENT2);
}

void GBuffer::readText() {
	glReadBuffer(GL_COLOR_ATTACHMENT3);
}