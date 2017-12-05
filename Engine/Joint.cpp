#include "Joint.h"

#include <glm/glm.hpp>

Joint::Joint(string name, int pos) {
	this->name = name;
	this->pos = pos;
}

void Joint::addMatrices(vector<glm::mat4> &matrices) {
	matrices[pos] = transform;
	for (unsigned int i = 0; i < children.size(); i++) {
		children[i]->addMatrices(matrices);
	}
}

void Joint::calcBindMatrix(glm::mat4 &parent) {
	glm::mat4 local = this->transform;
	this->transform = glm::inverse(parent * local);
	for (unsigned int i = 0; i < children.size(); i++) {
		children[i]->calcBindMatrix(local);
	}
}