#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 dir, glm::vec3 color) {
	this->dir = dir;
	this->color = color;
}