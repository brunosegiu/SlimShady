#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/vec3.hpp>

using namespace std;

class Material {
	public:
		glm::vec3 ambient, diffuse, specular;
		float transparency;
		bool isTextured;
		GLuint textID;
		bool isNormalMapped;
		GLuint normalID;
	public:
		Material();
		Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, std::string text, std::string normal);
		~Material();
		static std::vector<Material*> Material::loadMtl(std::string path);
		static GLuint loadTexture(std::string path);
};
