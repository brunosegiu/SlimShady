#include "Sun.h"
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

Sun::Sun(clock_t time) { //Puede ser inicializado con cualquier basura
	float t = time / double(CLOCKS_PER_SEC);
	this->light = new DirectionalLight(glm::vec3(-cos(t), -sin(t), 0.0), glm::vec3(1.0, 1.0, 1.0));
	this->moon = new DirectionalLight(glm::vec3(0.0, -1.0, 0.0), glm::vec3(1.0,1.0,0.7));
	lastDraw = 0;
	state = day;
	stop = false;
}

void Sun::updateLight(float time) {
	if (!stop) {
		switch (state) {
		case day:
			this->lastDraw = this->lastDraw + time;
			if (this->lastDraw >= M_PI) {
				lastDraw = 0;
				state = day_night;
			}
			else {
				float t = this->lastDraw;
				this->light->dir = glm::vec3(-cos(t), -sin(t), 0.0);
				this->light->color = glm::vec3(1.0, 1.0, 1.0)*(float)pow(sin(t), 2) + glm::vec3(1.0, 0.58, 0.16)*(float)pow(cos(t), 2);
				intensity = 1;
				mIntensity = 0;
			};
			break;
		case day_night:
			this->lastDraw = this->lastDraw + time;
			//printf("day_night \n");
			if (lastDraw >= M_PI / 2) {		//Me pierdo un frame. Si se nota mucho puedo poner el metodo del estado siguiente dentro de este if
				lastDraw = 0;
				state = night;
			}
			else {
				float t = lastDraw;
				light->dir = glm::vec3(1.0, 0.0, 0.0);
				light->color = glm::vec3(1.0, 0.58, 0.16);
				intensity = pow(cos(t), 2);						//Puedo usar intensity y 1-intensity para los parametros del skybox
				mIntensity = pow(sin(t), 2)*0.4;
			};
			break;
		case night:
			//printf("night \n");
			this->lastDraw = this->lastDraw + time;
			if (lastDraw >= M_PI) {
				lastDraw = 0;
				state = night_day;
			}
			else {
				intensity = 0;
				mIntensity = 0.4;
			};
			break;
		case night_day:
			//printf("night_day \n");
			this->lastDraw = this->lastDraw + time;
			if (lastDraw >= M_PI / 2) {
				lastDraw = 0;
				state = day;
			}
			else {
				float t = lastDraw;
				light->dir = glm::vec3(-1.0, 0.0, 0.0);
				light->color = glm::vec3(1.0, 0.58, 0.16);
				intensity = pow(sin(t), 2);						//Puedo usar intensity y 1-intensity para los parametros del skybox
				mIntensity = pow(cos(t), 2)*0.4;
			};
			break;
		}
	}



	/*this->lastDraw = this->lastDraw + time;
	if (lastDraw >= M_PI) {
		lastDraw = lastDraw - M_PI;
	}
	float t = lastDraw; //No quiero escribir lastDraw por todos lados
	if (isTrans) {
		
	}
	else {
		this->light->dir = glm::vec3(-cos(t), -sin(t), 0.0);
		if (light->dir.y > 0) {
			isTrans = true;
			this->transition = clock();
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
		}
	}
	//printf("%f, %f, %f\n", this->light->color.r, this->light->color.g, this->light->color.b);
	/*light->dir = glm::vec3(0.0, -1.0, 0.0);
	intensity = 1;*/
}

Sun::~Sun() {
	delete this->light;
	delete this->moon;
}