#include "World.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

World::World(Camera* cam) {
	this->cam = cam;
	
	//Shaders
	this->basic = new ShaderProgram("assets/shaders/basic.vert", "assets/shaders/basic.frag");
	this->veryBasic = new ShaderProgram("assets/shaders/veryBasic.vert", "assets/shaders/veryBasic.frag");
	this->geomertyPassShader = new ShaderProgram("assets/shaders/geometryPass.vert", "assets/shaders/geometryPass.frag");
	this->gbuf = new GBuffer(cam->width, cam->height);
}

void World::geomertyPass() {
	this->gbuf->bindW();
	this->geomertyPassShader->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLuint worldTransformID = glGetUniformLocation(geomertyPassShader->getId(), "worldTransform");
	GLuint textID = glGetUniformLocation(geomertyPassShader->getId(), "textSampler");
	GLuint modelTransformID = glGetUniformLocation(geomertyPassShader->getId(), "modelTransform");
	glUniform1i(textID, 0);
	for (unsigned int i = 0; i < worldEntities.size(); i++) {
		glm::mat4 toWorldCoords = this->cam->modelViewProjectionMatrix * this->worldEntities[i]->modelMatrix;
		glm::mat4 modelTransf = this->worldEntities[i]->modelMatrix;
		glUniformMatrix4fv(modelTransformID, 1, GL_FALSE, &modelTransf[0][0]);
		glUniformMatrix4fv(worldTransformID, 1, GL_FALSE, &toWorldCoords[0][0]);
		this->worldEntities[i]->draw(geomertyPassShader->getId());
	}
}

void World::lightPass() {
	unsigned int width, height;
	width = unsigned int(this->cam->width);
	height = unsigned int(this->cam->height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->gbuf->bindR();

	GLsizei hwidth = (GLsizei)(this->cam->width / 2.0f);
	GLsizei hheight = (GLsizei)(this->cam->height / 2.0f);

	this->gbuf->readPos();
	glBlitFramebuffer(0, 0, width, height,0, 0, hwidth, hheight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	this->gbuf->readDiff();
	glBlitFramebuffer(0, 0, width, height,0, hheight, hwidth, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	this->gbuf->readNorm();
	glBlitFramebuffer(0, 0, width, height, hwidth, hheight, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	this->gbuf->readText();
	glBlitFramebuffer(0, 0, width, height, hwidth, 0, width, hheight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void World::draw() {
	this->cam->update();
	geomertyPass();
	lightPass();
}

void World::dummyDraw() {
	this->cam->update();
	this->basic->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLuint worldTransformID = glGetUniformLocation(basic->getId(), "worldTransform");
	GLuint textID = glGetUniformLocation(basic->getId(), "textSampler");
	GLuint modelTransformID = glGetUniformLocation(basic->getId(), "modelTransform");
	glUniform1i(textID, 0);
	for (unsigned int i = 0; i < worldEntities.size(); i++) {
		this->worldEntities[i]->rotate(0.04f, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 toWorldCoords = this->cam->modelViewProjectionMatrix * this->worldEntities[i]->modelMatrix;
		glm::mat4 modelTransf = this->worldEntities[i]->modelMatrix;
		glUniformMatrix4fv(modelTransformID, 1, GL_FALSE, &modelTransf[0][0]);
		glUniformMatrix4fv(worldTransformID, 1, GL_FALSE, &toWorldCoords[0][0]);
		this->worldEntities[i]->draw(basic->getId());
	}

	this->veryBasic->bind();
	worldTransformID = glGetUniformLocation(veryBasic->getId(), "worldTransform");
	for (unsigned int j = 0; j < meshes.size(); j++) {
		glm::mat4 toWorldCoords = this->cam->modelViewProjectionMatrix;
		glUniformMatrix4fv(worldTransformID, 1, GL_FALSE, &toWorldCoords[0][0]);
		this->meshes[j].draw(veryBasic->getId());
	}
}



World::~World() {
	for (unsigned int i = 0; i < worldEntities.size(); i++) {
		delete this->worldEntities[i];
	}
	for (unsigned int i = 0; i < dirLights.size(); i++) {
		delete this->dirLights[i];
	}
	delete cam;
	delete basic;

	delete this->geomertyPassShader;
	delete gbuf;
}