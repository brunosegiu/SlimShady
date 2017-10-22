#pragma once

#include <string>

#include <GL/glew.h>
#include<glm/mat4x4.hpp>

class Entity {
	public:
		Entity();
		void rotate(float angle, glm::vec3 &dir);
		void translate(glm::vec3 &trs);
		void scale(glm::vec3 &scale);
		void scaleInOrigin(glm::vec3 &scale);
		virtual void draw(GLuint shaderID) = 0;
		~Entity();

		glm::mat4 modelMatrix;
};