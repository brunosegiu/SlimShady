#pragma once

#include <glm/vec3.hpp>

class DirectionalLight{
	public:
		glm::vec3 color;
		glm::vec3 dir;
	public:
		DirectionalLight(glm::vec3 dir, glm::vec3 color);

};
