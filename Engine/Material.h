#pragma once

#include <string>

#include <GL/glew.h>
#include <glm/vec3.hpp>

class Material {
	public:
		glm::vec3 ambient, diffuse, specular;
		float transparency;
		bool isTextured;
		GLuint textID;
	public:
		Material();
		Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, std::string text);
		~Material();
};
