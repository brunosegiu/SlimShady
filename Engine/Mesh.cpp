#include "Mesh.h"

#include <exception>
#include <iostream>
#include<fstream>
#include<string>
#include<iterator>

#include <glm/gtc/type_ptr.hpp>

#include "OBJIndexer.h"

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
		
		OBJIndexer indexer;

		vector<unsigned int> finalVertexIndices;

		int connectedVert = 0;
		int textureCoords = 0;
		int normalsCount = 0;

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

				vector<unsigned int> vertexIndices, normalIndices, textureIndices;
				push(vertexIndices, 3);
				push(normalIndices, 3);
				push(textureIndices, 3);
				
				if (textureCoords > 0) {
					push(textureIndices, 3);
					sscanf_s(line.c_str(), "%i/%i/%i %i/%i/%i %i/%i/%i",
						&vertexIndices[0], &textureIndices[0], &normalIndices[0],
						&vertexIndices[0 + 1], &textureIndices[0 +1], &normalIndices[0 +1],
						&vertexIndices[0 + 2], &textureIndices[0 +2], &normalIndices[0 +2]);
				}
				else {
					sscanf_s(line.c_str(), "%i//%i %i//%i %i//%i",
						&vertexIndices[0], &normalIndices[0],
						&vertexIndices[0+1], &normalIndices[0 +1],
						&vertexIndices[0+2], &normalIndices[0 +2]);
				}

				for (unsigned int i = 0; i < 3; i++) {
					vertexIndices[i] -= 1;
					normalIndices[i] -= 1;
					textureIndices[i] -= 1;
					unsigned int index = indexer.getIndex(vertexBuffer[3 * vertexIndices[i]], vertexBuffer[3 * vertexIndices[i] + 1], vertexBuffer[3 * vertexIndices[i] + 2],
														  normalBuffer[3 * normalIndices[i]], normalBuffer[3 * normalIndices[i] + 1], normalBuffer[3 * normalIndices[i] + 2],
														  textureBuffer[2*textureIndices[i]], textureBuffer[2*textureIndices[i] + 1]);
					finalVertexIndices.push_back(index);
				}
			}
			else if (!strncmp(line.c_str(), prefix.c_str(), strlen(prefix.c_str()))) {
				inserted++;
				// No more vertices, normals or texture coords to load, gen arrays on GPU
				if (currentMtl != "") {
					// Filled up an index array, copy it.
					this->indices.push_back(0);
					this->faces.push_back(finalVertexIndices.size());
					glGenBuffers(1, &this->indices[this->indices.size() - 1]);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indices[this->indices.size() - 1]);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * finalVertexIndices.size(), &finalVertexIndices[0], GL_STATIC_DRAW);
					finalVertexIndices.clear();
				}
				currentMtl = line.substr(7, line.length());
			}
		}

		//Copy vertices to GPU
		glGenBuffers(1, &this->verticesID);
		glBindBuffer(GL_ARRAY_BUFFER, this->verticesID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * indexer.vertices.size(), &indexer.vertices[0], GL_STATIC_DRAW);

		//Copy normals to GPU
		glGenBuffers(1, &this->normalsID);
		glBindBuffer(GL_ARRAY_BUFFER, this->normalsID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * indexer.normals.size(), &indexer.normals[0], GL_STATIC_DRAW);

		//Copy UV to GPU
		if (textureCoords > 0) {
			glGenBuffers(1, &this->textID);
			glBindBuffer(GL_ARRAY_BUFFER, this->textID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * indexer.textures.size(), &indexer.textures[0], GL_STATIC_DRAW);
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
			this->faces.push_back(finalVertexIndices.size());
			glGenBuffers(1, &this->indices[this->indices.size() - 1]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indices[this->indices.size() - 1]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * finalVertexIndices.size(), &finalVertexIndices[0], GL_STATIC_DRAW);
			finalVertexIndices.clear();
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