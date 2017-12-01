#pragma once

#include <time.h>

#include <glm/vec3.hpp>

#include "FreeMesh.h"
#include "ShaderProgram.h"
#include "Model.h"

class FreeMesh;

class Water : public Model{
public:
	FreeMesh* mesh;
	ShaderProgram* shader;
	GLuint textureID, normalID;

	//States
	glm::mat4 mvp;
	clock_t lastDraw;
	glm::vec3 camPos;
	glm::vec3 lightDir;  
	glm::vec3 lightColor;
	float intensity;
	glm::vec3 moonDir;
	glm::vec3 moonColor;
	float mIntensity;
public:
	Water(int width, int height);
	GLuint loadTexture(std::string path);
	void draw(GLuint shaderID);
	~Water();
};