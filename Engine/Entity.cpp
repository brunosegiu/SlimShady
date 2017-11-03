#include "Entity.h"

#include <glm/gtx/transform.hpp>

Entity::Entity(Model* model, Camera* cam) {
	this->modelMatrix = glm::mat4(1.0f);
	this->model = model;
	this->cam = cam;
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
	this->modelMatrix = glm::translate(acumulatedTranslate) * glm::scale(this->acumulatedScale) * glm::rotate(acumulatedRotate.x, glm::vec3(1.0f,0.0f,0.0f)) * glm::rotate(acumulatedRotate.y, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(acumulatedRotate.z, glm::vec3(0.0f, 0.0f, 1.0f));
	GLuint worldTransformID = glGetUniformLocation(shaderID, "worldTransform");
	glm::mat4 toWorldCoords = this->cam->modelViewProjectionMatrix * this->modelMatrix;
	glUniformMatrix4fv(worldTransformID, 1, GL_FALSE, &toWorldCoords[0][0]);

	GLuint textID = glGetUniformLocation(shaderID, "textSampler");
	glUniform1i(textID, 0);

	GLuint normTextID = glGetUniformLocation(shaderID, "normTextSampler");
	if (normTextID != -1)
		glUniform1i(normTextID, 1);
	
	GLuint modelTransformID = glGetUniformLocation(shaderID, "modelTransform");
	if (modelTransformID != -1)
		glUniformMatrix4fv(modelTransformID, 1, GL_FALSE, &modelMatrix[0][0]);

	GLuint modelViewID = glGetUniformLocation(shaderID, "modelView");
	glUniformMatrix3fv(modelViewID, 1, GL_FALSE, &glm::mat3(this->cam->viewMatrix * this->modelMatrix)[0][0]);

	this->model->draw(shaderID);
	
}

Entity::~Entity() {
}