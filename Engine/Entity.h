#pragma once

#include <string>

#include <GL/glew.h>
#include<glm/mat4x4.hpp>

#include "Model.h"
#include "World.h"

class World;

class Entity {
	public:
		Entity(Model* model, World* world);
		void rotate(float angle, glm::vec3 &dir);
		void translate(glm::vec3 &trs);
		void scale(glm::vec3 &scale);
		void draw(GLuint shaderID);
		~Entity();
		
		Model* model;
		World* world;
		glm::mat4 modelMatrix;
		glm::vec3 acumulatedTranslate;
		glm::vec3 acumulatedRotate;
		glm::vec3 acumulatedScale;
};