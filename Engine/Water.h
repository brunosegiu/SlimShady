#pragma once

#include <time.h>

#include <glm/vec3.hpp>

#include "FreeMesh.h"
#include "ShaderProgram.h"

class Water{
public:
	FreeMesh* mesh;
	ShaderProgram* shader;

	//States
	glm::mat4 mvp;
	clock_t lastDraw;
	glm::vec3 camPos;
public:
	Water(int width, int height);
	void draw(GLuint shaderID);
	~Water();
};