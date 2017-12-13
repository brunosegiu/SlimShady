#pragma once
#include "Model.h"
#include <vector>
#include "ShaderProgram.h"
#include <glm\glm.hpp>
class Grass : public Model{
public:
	GLuint vaoID, vboID, texID;
	float fGrassPatchOffsetMin;// = 1.5f;
	float fGrassPatchOffsetMax;// = 2.5f;
	float fGrassPatchHeight;// = 5.0f;
	std::vector<float> positions;
	ShaderProgram* shader;
	int sizeX, sizeY, density, vertexCount;
	glm::mat4 modelMatrix, viewMatrix, projMatrix;
	glm::vec3 l1, i1, l2, i2;
	float time;
	glm::vec3 vEyePosition;
public:
	Grass(std::vector<float> positions, std::string texPath, int sizeX, int sizeY, int density);
	virtual ~Grass();
	GLuint loadTexture(std::string path);
	void update(float time, glm::vec3 vEyePosition);
	void draw(GLuint shaderID);
};

