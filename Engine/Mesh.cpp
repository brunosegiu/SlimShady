#include "Mesh.h"

#include <exception>
#include <iostream>
#include<fstream>
#include<string>
#include<iterator>

#include <glm/gtc/type_ptr.hpp>

void push(std::vector<float> &v, int cant) {
	for (int i = 0; i < cant; i++) {
		v.push_back(0);
	}
}

void push(std::vector<unsigned int> &v, int cant) {
	for (int i = 0; i < cant; i++) {
		v.push_back(0);
	}
}

Mesh::Mesh(string path) : Entity() {
	this->path = path;
	string fullpath = path + ".obj";
	this->mats = Material::loadMtl(path);
	string line;
	ifstream objFile(fullpath.c_str());
	if (objFile.is_open()) {
		objFile.seekg(0, ios::end);
		long fileSize = long(objFile.tellg());
		objFile.seekg(0, ios::beg);

		unsigned int inserted = 0;

		string currentMtl = "";

		vector<float> vertexBuffer;
		vector<float> textureBuffer;
		vector<float> normalBuffer;
		vector<float> finalNormalBuffer;
		vector<float> finalTextureBuffer;

		vector<unsigned int> vertexIndices;
		vector<unsigned int> normalIndices;
		vector<unsigned int> textureIndices;

		int connectedVert = 0;
		int textureCoords = 0;
		int normalsCount = 0;

		int vertexIndex = 0;

		string prefix = "usemtl";

		while (!objFile.eof()) {
			getline(objFile, line);
			if (line.c_str()[0] == 'v' && line.c_str()[1] == ' ') {
				line[0] = ' ';
				push(vertexBuffer, 3);
				sscanf_s(line.c_str(), "%f %f %f ",
					&vertexBuffer[connectedVert],
					&vertexBuffer[connectedVert + 1],
					&vertexBuffer[connectedVert + 2]);
				connectedVert += 3;
			}
			else if (line.c_str()[0] == 'v' && line.c_str()[1] == 't') {
				line[0] = ' ';
				line[1] = ' ';
				push(textureBuffer, 2);
				sscanf_s(line.c_str(), "%f %f ",
					&textureBuffer[textureCoords],
					&textureBuffer[textureCoords + 1]);
				textureCoords += 2;
			}
			else if (line.c_str()[0] == 'v' && line.c_str()[1] == 'n') {
				line[0] = ' ';
				line[1] = ' ';
				push(normalBuffer, 3);
				sscanf_s(line.c_str(), "%f %f %f ",
					&normalBuffer[normalsCount],
					&normalBuffer[normalsCount + 1],
					&normalBuffer[normalsCount + 2]);
				normalsCount += 3;
			}
			else if (line.c_str()[0] == 'f') {
				line[0] = ' ';

				push(vertexIndices, 3);
				push(normalIndices, 3);
				push(textureIndices, 3);
				
				if (textureCoords > 0) {
					push(textureIndices, 3);
					sscanf_s(line.c_str(), "%i/%i/%i %i/%i/%i %i/%i/%i",
						&vertexIndices[vertexIndex], &textureIndices[vertexIndex], &normalIndices[vertexIndex],
						&vertexIndices[vertexIndex + 1], &textureIndices[vertexIndex +1], &normalIndices[vertexIndex +1],
						&vertexIndices[vertexIndex + 2], &textureIndices[vertexIndex +2], &normalIndices[vertexIndex +2]);
				}
				else {
					sscanf_s(line.c_str(), "%i//%i %i//%i %i//%i",
						&vertexIndices[vertexIndex], &normalIndices[vertexIndex],
						&vertexIndices[vertexIndex+1], &normalIndices[vertexIndex +1],
						&vertexIndices[vertexIndex+2], &normalIndices[vertexIndex +2]);
				}

				// Indices are offset by 1
				for (int j = 0; j < 3; j++) {
					vertexIndices[vertexIndex+j] -= 1;
					normalIndices[vertexIndex +j] -= 1;
					textureIndices[vertexIndex +j] -= 1;
				}
				
				if (finalNormalBuffer.size() < vertexBuffer.size()) {
					push(finalNormalBuffer, vertexBuffer.size() - finalNormalBuffer.size());
				}
				// Since OpenGL doesn't support normal/texture indirection, set a normal per vertex (and a text coord)
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						finalNormalBuffer[3 * (vertexIndices[vertexIndex+i]) + j] = normalBuffer[3 * normalIndices[vertexIndex + i] + j];
					}
				}
				if (textureCoords > 0) {
					if (finalTextureBuffer.size() < vertexBuffer.size()) {
						push(finalTextureBuffer, 2*vertexBuffer.size()/3 - finalTextureBuffer.size());
					}
					for (int i = 0; i < 3; i++) {
						for (int j = 0; j < 2; j++) {
							finalTextureBuffer[2 * (vertexIndices[vertexIndex + i]) + j] = textureBuffer[2 * textureIndices[vertexIndex + i] + j];
						}
					}
				}

				vertexIndex += 3;
			}
			else if (!strncmp(line.c_str(), prefix.c_str(), strlen(prefix.c_str()))) {
				inserted++;
				// No more vertices, normals or texture coords to load, gen arrays on GPU
				if (currentMtl != "") {
					// Filled up an index array, copy it.
					this->indices.push_back(0);
					this->faces.push_back(vertexIndex);
					glGenBuffers(1, &this->indices[this->indices.size() - 1]);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indices[this->indices.size() - 1]);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * vertexIndex, &vertexIndices[0], GL_STATIC_DRAW);
				}
				currentMtl = line.substr(7, line.length());
				vertexIndex = 0;
			}
		}

		//Copy vertices to GPU
		glGenBuffers(1, &this->verticesID);
		glBindBuffer(GL_ARRAY_BUFFER, this->verticesID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * connectedVert, &vertexBuffer[0], GL_STATIC_DRAW);

		//Copy normals to GPU
		glGenBuffers(1, &this->normalsID);
		glBindBuffer(GL_ARRAY_BUFFER, this->normalsID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * connectedVert, &finalNormalBuffer[0], GL_STATIC_DRAW);

		//Copy UV to GPU
		if (textureCoords > 0) {
			glGenBuffers(1, &this->textID);
			glBindBuffer(GL_ARRAY_BUFFER, this->textID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * (connectedVert / 3), &finalTextureBuffer[0], GL_STATIC_DRAW);
		}

		//Set up the VAO
		glGenVertexArrays(1, &vaoID);
		glBindVertexArray(vaoID);
		//Bind Vertices
		glBindBuffer(GL_ARRAY_BUFFER, this->verticesID);
		glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		//Bind Normals
		glBindBuffer(GL_ARRAY_BUFFER, this->normalsID);
		glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		//Bind textures
		if (textureCoords > 0) {
			glBindBuffer(GL_ARRAY_BUFFER, this->textID);
			glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}
			
		if (currentMtl != "" && indices.size() < inserted) {
			// Copy indices since the last one wont be copied.
			this->indices.push_back(0);
			this->faces.push_back(vertexIndex);
			glGenBuffers(1, &this->indices[this->indices.size() - 1]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indices[this->indices.size() - 1]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * vertexIndex, &vertexIndices[0], GL_STATIC_DRAW);
		}

		objFile.close();
		this->textures = textureCoords > 0;
	}
	else {
		throw runtime_error("Unable to load obj file: " + path + ".obj");
	}
}

void Mesh::draw(GLuint shaderID) {
	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	if (textures) {
		glActiveTexture(GL_TEXTURE0);
		glEnableVertexAttribArray(2);
	}
	GLuint textured = glGetUniformLocation(shaderID, "isTextured");
	GLuint kd = glGetUniformLocation(shaderID, "Kd");
	for (unsigned int i = 0; i < this->indices.size(); i++) {
		glUniform1i(textured, this->mats[i]->isTextured);
		if (this->mats[i]->isTextured) {
			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->mats[i]->textID);
		}
		if (this->mats[i]->isNormalMapped) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, this->mats[i]->normalID);
		}
		glUniform3f(kd,this->mats[i]->diffuse.x, this->mats[i]->diffuse.y, this->mats[i]->diffuse.z);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indices[i]);
		glDrawElements(GL_TRIANGLES,this->faces[i],	GL_UNSIGNED_INT,(void*)0);
		if (this->mats[i]->isTextured) {
			glDisable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		if (this->mats[i]->isNormalMapped) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	if (textures)
		glDisableVertexAttribArray(2);
}

Mesh::~Mesh() {
	glDeleteBuffers(1, &this->verticesID);
	glDeleteBuffers(1, &this->normalsID);
	if (this->textures)
		glDeleteBuffers(1, &this->textID);
	glDeleteVertexArrays(1, &vaoID);
	for (unsigned int i = 0; i < this->indices.size(); i++) {
		glDeleteBuffers(1, &this->indices[i]);
		delete mats[i];
	}
}