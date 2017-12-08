#include "FreeMesh.h"
#include <exception>
#include <iostream>
#include<fstream>
#include<string>
#include<iterator>

#include <glm/gtc/type_ptr.hpp>

FreeMesh::FreeMesh(std::vector<float> positions, std::vector<unsigned int> index){
	vertexCount = positions.size() / 3;
	indexCount = index.size();

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	//index buffer
	GLuint indices;
	glGenBuffers(1, &indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size()*sizeof(unsigned int), &index[0], GL_STATIC_DRAW);


	glGenBuffers(1, &verticesID);
	glBindBuffer(GL_ARRAY_BUFFER, this->verticesID);
	glBufferData(GL_ARRAY_BUFFER, positions.size()*sizeof(float), &positions[0], GL_STATIC_DRAW);
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

	textured = false;
}

void FreeMesh::draw(GLenum primitive) {
	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	if(textured) glEnableVertexAttribArray(1);
	glDrawElements(primitive, indexCount, GL_UNSIGNED_INT, (void*)0);
	glDisableVertexAttribArray(0);
	if(textured) glDisableVertexAttribArray(1);
}

void FreeMesh::addTexture(std::vector<float> textcoords) {
	glBindVertexArray(vaoID);
	glGenBuffers(1, &textureID);
	glBindBuffer(GL_ARRAY_BUFFER, this->textureID);
	glBufferData(GL_ARRAY_BUFFER, textcoords.size() * sizeof(float), &textcoords[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	textured = true;
}

void FreeMesh::setShader(GLuint shaderID) {
	this->shaderID = shaderID;
}

FreeMesh::~FreeMesh() {
	glDeleteBuffers(1, &this->verticesID);
	//glDeleteBuffers(1, &this->normalsID);
	glDeleteVertexArrays(1, &vaoID);
}
