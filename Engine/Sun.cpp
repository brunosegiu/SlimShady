#include "Sun.h"
#include <math.h>
#include <stdio.h>

Sun::Sun(clock_t time) { //Puede ser inicializado con cualquier basura
	float t = time / double(CLOCKS_PER_SEC);
	this->light = new DirectionalLight(glm::vec3(-cos(t), -sin(t), 0.0), glm::vec3(1.0, 1.0, 1.0));
}

void Sun::updateLight(clock_t time) {
	float t = time / (double(CLOCKS_PER_SEC)*5);
	this->light->dir = glm::vec3(-cos(t), -sin(t), 0.0);
	this->light->color = glm::vec3(0.1, 0.1, 1.0)*(float)pow(sin(t),2) + glm::vec3(1.0, 0.58, 0.16)*(float)pow(cos(t),2);
	//printf("%f, %f, %f\n", this->light->color.r, this->light->color.g, this->light->color.b);
}

Sun::~Sun() {
	delete this->light;
}