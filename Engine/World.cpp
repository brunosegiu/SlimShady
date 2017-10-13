#include "World.h"

World::World(Camera* cam) {
	this->cam = cam;
	
	//Shaders
	this->basic = new ShaderProgram("assets/shaders/basic.vert", "assets/shaders/basic.frag");
}

void World::draw() {
	this->cam->update();
	this->basic->bind();
	GLuint worldTransformID = glGetUniformLocation(basic->getId(), "worldTransform");
	GLuint textID = glGetUniformLocation(basic->getId(), "textSampler");
	glUniform1i(textID, 0);
	for (unsigned int i = 0; i < worldEntities.size(); i++) {
		glm::mat4 toWorldCoords = this->cam->modelViewProjectionMatrix * this->worldEntities[i]->modelMatrix;
		glUniformMatrix4fv(worldTransformID, 1, GL_FALSE, &toWorldCoords[0][0]);
		this->worldEntities[i]->draw(basic->getId());
	}
	this->basic->unbind();
}

World::~World() {
	for (unsigned int i = 0; i < worldEntities.size(); i++) {
		delete this->worldEntities[i];
	}
	delete cam;
	delete basic;
}