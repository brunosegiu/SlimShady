#include "Camera.h"

#define _USE_MATH_DEFINES
#include <cmath> 

#include <GL/glew.h>
#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <stdio.h>

Camera::Camera(float width, float height, float fov, SDL_Window* win) {
	this->pos = glm::vec3(5, 5, 5);
	this->ref = glm::vec3(0, 0, 0);
	this->up = glm::vec3(0, 1, 0);
	this->fov = fov;
	this->in = false;
	this->moveSpeed = 50.0f;
	this->sensitivity = 80.0f;
	this->width = width;
	this->height = height;
	this->win = win;
	this->lastUpdate = std::clock();
	this->set = false;

	this->projectionMatrix = glm::perspective(glm::radians(fov), width / height, 0.5f, 1000.0f);
	this->viewMatrix = glm::lookAt(pos, ref, up);
	this->modelViewProjectionMatrix = this->projectionMatrix * this->viewMatrix;
}

Camera::Camera(glm::vec3 pos, glm::vec3 ref, glm::vec3 up, float fov, float sensitivity, float speed, SDL_Window* win, float width, float height) {
	this->pos = pos;
	this->ref = ref;
	this->up = up;
	this->in = false;
	this->fov = fov;
	this->moveSpeed = speed;
	this->sensitivity = sensitivity;
	this->width = width;
	this->height = height;
	this->win = win;
	this->lastUpdate = std::clock();
	this->set = false;

	this->projectionMatrix = glm::perspective(glm::radians(fov), width / height, 1.0f, 100.0f);
	this->viewMatrix = glm::lookAt(pos, ref, up);
	this->modelViewProjectionMatrix = this->projectionMatrix * this->viewMatrix;
}

void Camera::update() {
	if (!set) {
		set = true;
		SDL_WarpMouseInWindow(win, unsigned int(width / 2), unsigned int(height / 2));
	}
	if (in) {
		float dur = float((std::clock() - lastUpdate) / (double)CLOCKS_PER_SEC);
		const Uint8* state = SDL_GetKeyboardState(NULL);
		glm::vec3 dir = glm::normalize(ref - pos);
		glm::vec3 right = glm::cross(dir, up);
		if (state[SDL_SCANCODE_W]) {
			glm::vec3 fact = moveSpeed * dur * dir;
			this->pos += fact;
			this->ref += fact;
		}
		else if (state[SDL_SCANCODE_S]) {
			glm::vec3 fact = moveSpeed * dur * dir;
			this->pos -= fact;
			this->ref -= fact;
		}
		if (state[SDL_SCANCODE_A]) {
			glm::vec3 fact = moveSpeed * dur * right;
			this->pos -= fact;
			this->ref -= fact;
		}
		else if (state[SDL_SCANCODE_D]) {
			glm::vec3 fact = moveSpeed * dur * right;
			this->pos += fact;
			this->ref += fact;
		}
		int x, y;
		SDL_GetMouseState(&x, &y);
		if (x != mouseX || y != mouseY) {
			float dirX, dirY;
			dirX = sensitivity * dur * (mouseX - x)/width;
			dirY = sensitivity * dur * (mouseY - y)/height; // Inverted
			glm::vec3 newDir = glm::mat3(glm::rotate(dirX, up)) * dir;
			newDir = glm::mat3(glm::rotate(dirY, right)) * newDir;
			this->ref = pos + newDir;
		}
		SDL_WarpMouseInWindow(win, unsigned int (width / 2), unsigned int (height / 2));
		mouseX = unsigned int(width / 2);
		mouseY = unsigned int(height / 2);
	}
	this->viewMatrix = glm::lookAt(pos, ref, up);
	this->modelViewProjectionMatrix = this->projectionMatrix * this->viewMatrix;
	this->lastUpdate = clock();
}