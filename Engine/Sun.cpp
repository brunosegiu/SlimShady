#include "Sun.h"
#include <math.h>
#include <stdio.h>

Sun::Sun(clock_t time) { //Puede ser inicializado con cualquier basura
	float t = time / double(CLOCKS_PER_SEC);
	this->light = new DirectionalLight(glm::vec3(-cos(t), -sin(t), 0.0), glm::vec3(1.0, 1.0, 1.0));
	this->moon = new DirectionalLight(glm::vec3(0.0, -1.0, 0.0), glm::vec3(1.0,1.0,0.7));
}

void Sun::updateLight(clock_t time) {
	/*float t = time / (double(CLOCKS_PER_SEC)*5);
	this->light->dir = glm::vec3(-cos(t), -sin(t), 0.0);
	if (light->dir.y > 0) {
		if (light->dir.x < 0) {
			light->dir = glm::vec3(-1.0, 0.0, 0.0);
		}
		else if (light->dir.x > 0) {
			light->dir = glm::vec3(1.0, 0.0, 0.0);
		}
		light->color = glm::vec3(1.0, 0.58, 0.16);
		intensity = pow(cos(t), 2);
		mIntensity = pow(sin(t), 2)*0.4;
	}
	else {
		this->light->color = glm::vec3(1.0, 1.0, 1.0)*(float)pow(sin(t), 2) + glm::vec3(1.0, 0.58, 0.16)*(float)pow(cos(t), 2);
		intensity = 1;
		mIntensity = 0;
	}*/
	intensity = 1.0f;
		light->dir = glm::vec3(0, -1, 0);
	//printf("%f, %f, %f\n", this->light->color.r, this->light->color.g, this->light->color.b);
}

Sun::~Sun() {
	delete this->light;
}