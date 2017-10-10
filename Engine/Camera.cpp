#include "Camera.h"

#define _USE_MATH_DEFINES
#include <cmath> 

#include <GL/glew.h>
#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <stdio.h>

Camera::Camera(float width, float height, SDL_Window* win) {
	this->pos = glm::vec3(5, 5, 5);
	this->ref = glm::vec3(0, 0, 0);
	this->up = glm::vec3(0, 1, 0);
	this->in = false;
	this->moveSpeed = 5.0f;
	this->sensitivity = 800.0f;
	this->width = width;
	this->height = height;
	this->win = win;
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	gluLookAt(pos.x, pos.y, pos.z, ref.x, ref.y, ref.z, up.x, up.y, up.z);
	this->lastUpdate = std::clock();
}

void Camera::update() {
	if (in) {
		float dur = (std::clock() - lastUpdate) / (double)CLOCKS_PER_SEC;
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
	gluLookAt(pos.x, pos.y, pos.z, ref.x, ref.y, ref.z, up.x, up.y, up.z);
	this->lastUpdate = clock();
}