#pragma once

#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <string>

#include "FreeMesh.h"
#include "ShaderProgram.h"

class FreeMesh;

class Skybox{
public:
	FreeMesh *front, *up, *right, *back, *down, *left;
	ShaderProgram* shader;
	GLuint frontID, upID, rightID, backID, downID, leftID;
	//states
	glm::mat4 mvp;
	glm::vec3 lightColor;
	float intensity;
	glm::vec3 moonColor;
	float mIntensity;
public:
	Skybox(int size);
	GLuint loadTexture(std::string path);
	void draw(GLuint shaderID);
	~Skybox();
};