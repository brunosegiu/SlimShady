#include "Entity.h"

Entity::Entity(Mesh* mesh) {
	this->mesh = mesh;
	this->modelMatrix = glm::mat4(1.0f);
}

void Entity::applyTransform(glm::mat4 &transform) {
	this->modelMatrix *= transform;
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