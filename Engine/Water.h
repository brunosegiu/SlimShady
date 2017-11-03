#pragma once

#include <time.h>

#include <glm/vec3.hpp>

#include "FreeMesh.h"
#include "ShaderProgram.h"
#include "Model.h"

class Water : public Model{
public:
	FreeMesh* mesh;
	ShaderProgram* shader;

	//States
	glm::mat4 mvp;
	clock_t lastDraw;
	glm::vec3 camPos;
	glm::vec3 lightDir;  //No, porque van a tener todos lo mismo
	glm::vec3 lightColor;
public:
	Water(int width, int height);
	void draw(GLuint shaderID);
	~Water();
};