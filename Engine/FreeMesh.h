#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>

class FreeMesh {
public:
	GLuint verticesID, normalsID, vaoID;
	unsigned int vertexCount;

public:
	FreeMesh(std::vector<float> positions/*, GLenum type, unsigned int grouping*/);
	void draw(GLuint shaderID);
	~FreeMesh();
};

/*
La gracia es que se pueda enchufar un conjunto de vertices y decirle que dibuje la primitiva q se te cante
se podrian calcular las normales a huevo o tambien pasarlas en el constructor
*/