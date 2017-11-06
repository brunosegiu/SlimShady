#include "Entity.h"
#include "MeshInstanced.h"
#include <iostream>
#include <glm/gtx/transform.hpp>

Entity::Entity(Model* model, World* world) {
	this->modelMatrix = glm::mat4(1.0f);
	this->model = model;
	this->world = world;
	this->acumulatedRotate = glm::vec3(0.0f, 0.0f, 0.0f);
	this->acumulatedTranslate = glm::vec3(0.0f, 0.0f, 0.0f);
	this->acumulatedScale = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Entity::rotate(float angle, glm::vec3 &dir) {
	this->acumulatedRotate += angle * dir;
}

void Entity::translate(glm::vec3 &trs) {
	this->acumulatedTranslate += trs;
}

void Entity::scale(glm::vec3 &scale) {
	this->acumulatedScale *= scale;
}

void Entity::draw(GLuint shaderID) {

	if (shaderID == 0) {
		Terrain* terr = dynamic_cast<Terrain*>(model);
		if (terr) {
			terr->terrainShader->bind();
		}
		shaderID = terr->terrainShader->getId();
	}
	this->modelMatrix = glm::translate(acumulatedTranslate) * glm::scale(this->acumulatedScale) * glm::rotate(acumulatedRotate.x, glm::vec3(1.0f,0.0f,0.0f)) * glm::rotate(acumulatedRotate.y, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(acumulatedRotate.z, glm::vec3(0.0f, 0.0f, 1.0f));
	GLuint worldTransformID = glGetUniformLocation(shaderID, "worldTransform");
	glm::mat4 toWorldCoords = this->world->cam->modelViewProjectionMatrix * this->modelMatrix;
	glUniformMatrix4fv(worldTransformID, 1, GL_FALSE, &toWorldCoords[0][0]);

	GLuint textID = glGetUniformLocation(shaderID, "textSampler");
	glUniform1i(textID, 0);

	GLuint normTextID = glGetUniformLocation(shaderID, "normTextSampler");
	if (normTextID != -1)
		glUniform1i(normTextID, 1);
	
	GLuint modelTransformID = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelTransformID, 1, GL_FALSE, &modelMatrix[0][0]);

	GLuint modelViewID = glGetUniformLocation(shaderID, "modelView");
	if (modelViewID != -1)
		glUniformMatrix3fv(modelViewID, 1, GL_FALSE, &glm::mat3(this->world->cam->viewMatrix * this->modelMatrix)[0][0]);

	GLuint lightDirID = glGetUniformLocation(shaderID, "lightdir");
	GLuint lightColorID = glGetUniformLocation(shaderID, "lightcolor");

	glUniform3fv(lightDirID, 1, &world->sun->light->dir[0]);
	glUniform3fv(lightColorID, 1, &world->sun->light->color[0]);

	MeshInstanced* meshi = dynamic_cast<MeshInstanced*>(this->model);
	if (meshi) {
		meshi->drawInstanced(shaderID);
	}
	else {
		this->model->draw(shaderID);
	}
}

Entity::~Entity() {
}