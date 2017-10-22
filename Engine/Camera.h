#pragma once

#include <ctime>

#include <glm/vec3.hpp>
#include <SDL.h>
#include <glm/mat4x4.hpp>

class Camera {
	public:
		glm::vec3 pos, ref, up;
		float width, height;
		bool in, set;
		float moveSpeed, sensitivity;
		int mouseX, mouseY;
		SDL_Window* win;
		std::clock_t lastUpdate;

		glm::mat4 modelViewProjectionMatrix;
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

		Camera(float width, float height, float fov, SDL_Window* win);
		void update();
};