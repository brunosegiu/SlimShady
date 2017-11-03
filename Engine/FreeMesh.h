#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>

#include "Entity.h"

class FreeMesh : public Entity {
public:
	GLuint verticesID, normalsID, vaoID, shaderID, textureID;
	unsigned int vertexCount, indexCount;
	bool textured;
public:
	FreeMesh(std::vector<float> positions, std::vector<unsigned int>/*, GLenum type, unsigned int grouping*/);
	void setShader(GLuint shaderID); //No se si poner un puntero al shader que usa
	void draw(GLuint shaderID);
	void addTexture(std::vector<float> textcoords);
	~FreeMesh();
};

/*
La gracia es que se pueda enchufar un conjunto de vertices y decirle que dibuje la primitiva q se te cante
se podrian calcular las normales a huevo o tambien pasarlas en el constructor
*/