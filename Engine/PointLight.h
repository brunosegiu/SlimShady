#pragma once

#include <glm/vec3.hpp>

class PointLight {
	public:
		glm::vec3 color;
		glm::vec3 position;
	public:
		PointLight(glm::vec3 position, glm::vec3 color);

};