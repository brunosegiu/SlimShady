#include "Sun.h"
#include <math.h>

Sun::Sun(clock_t time) { //Puede ser inicializado con cualquier basura
	float t = time / double(CLOCKS_PER_SEC);
	this->light = new DirectionalLight(glm::vec3(-cos(t), -sin(t), 0.0), glm::vec3(1.0, 1.0, 1.0));
}

void Sun::updateLight(clock_t time) {
	float t = time / (double(CLOCKS_PER_SEC)*5);
	this->light->dir = glm::vec3(-cos(t), -sin(t), 0.0);
}

Sun::~Sun() {
	delete this->light;
}