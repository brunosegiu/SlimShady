#pragma once

#include <glm/vec3.hpp>

class Material {
	public:
		glm::vec3 ambient, diffuse, specular;
		float transparency;
	public:
		Material();
		Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
};
