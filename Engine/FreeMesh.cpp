#include "FreeMesh.h"
#include <exception>
#include <iostream>
#include<fstream>
#include<string>
#include<iterator>

#include <glm/gtc/type_ptr.hpp>

FreeMesh::FreeMesh(std::vector<float> positions) {
	vertexCount = positions.size() / 3;

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glGenBuffers(1, &verticesID);
	glBindBuffer(GL_ARRAY_BUFFER, this->verticesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, positions.size()*sizeof(float), &positions[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/*std::vector<float> normales;
	for (unsigned int i = 0; i < vertexCount; i++) {
		normales.push_back(0.0f);
		normales.push_back(1.0f);
		normales.push_back(0.0f);
	}

	glGenBuffers(1, &this->normalsID);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalsID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, normales.size() * sizeof(float), &normales[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);*/

}

void FreeMesh::draw(GLuint shaderID) {
	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
}

FreeMesh::~FreeMesh() {
	glDeleteBuffers(1, &this->verticesID);
	//glDeleteBuffers(1, &this->normalsID);
	glDeleteVertexArrays(1, &vaoID);
}
