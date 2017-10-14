#include "Entity.h"

#include <glm/gtx/transform.hpp>

Entity::Entity(Mesh* mesh) {
	this->mesh = mesh;
	this->modelMatrix = glm::mat4(1.0f);
	this->normalRotations = glm::mat4(1.0f);
}

void Entity::rotate(float angle, glm::vec3 &dir) {
	this->modelMatrix *= glm::rotate(angle, dir);
	this->normalRotations *= glm::rotate(angle, dir);
}

void Entity::translate(glm::vec3 &trs) {
	this->modelMatrix = this->modelMatrix * glm::translate(trs);
}

void Entity::scale(glm::vec3 &scale) {
	this->modelMatrix = glm::scale(this->modelMatrix, scale);
}

void Entity::scaleInOrigin(glm::vec3 &scale) {
	//Need entity origin
}

void Entity::draw(GLuint shaderID) {
	if (mesh) {
		mesh->draw(shaderID);
	}
	else if (false) {
		//anim.draw();
	}
}
Entity::~Entity() {
	if (mesh)
		delete mesh;
}