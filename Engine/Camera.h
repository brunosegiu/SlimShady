#pragma once

#include <ctime>

#include <glm/vec3.hpp>
#include <SDL.h>

class Camera {
	public:
		glm::vec3 pos, ref, up;
		float yaw, pitch;
		float width, height;
		bool in;
		float moveSpeed, sensitivity;
		int mouseX, mouseY;
		SDL_Window* win;
		std::clock_t lastUpdate;
		Camera(float width, float height, SDL_Window* win);
		void update();
};