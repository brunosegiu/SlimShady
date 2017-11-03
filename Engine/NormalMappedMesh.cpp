#include "NormalMappedMesh.h"

#include <exception>
#include <iostream>
#include<fstream>
#include<string>
#include<iterator>

#include <glm/gtc/type_ptr.hpp>

#include "OBJIndexer.h"

void NormalMappedMesh::push(std::vector<float> &v, int cant) {
	for (int i = 0; i < cant; i++) {
		v.push_back(0);
	}
}

void NormalMappedMesh::push(std::vector<unsigned int> &v, int cant) {
	for (int i = 0; i < cant; i++) {
		v.push_back(0);
	}
}

NormalMappedMesh::NormalMappedMesh(string path){
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
		vector<float> tangentBuffer;

		vector<unsigned int> finalVertexIndices;

		OBJIndexer indexer;

		int connectedVert = 0;
		int textureCoords = 0;
		int normalsCount = 0;

		string prefix = "usemtl";
		string name = "None";

		while (!objFile.eof()) {
			getline(objFile, line);
			if (line.c_str()[0] == 'o' && line.c_str()[1] == ' ') {
				line[0] = ' ';
				name = line.substr(2, line.size());
			}
			else if (line.c_str()[0] == 'v' && line.c_str()[1] == ' ') {
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
				vector<unsigned int> vertexIndices;
				vector<unsigned int> normalIndices;
				vector<unsigned int> textureIndices;
				push(vertexIndices, 3);
				push(normalIndices, 3);
				push(textureIndices, 3);

				if (textureCoords > 0) {
					push(textureIndices, 3);
					sscanf_s(line.c_str(), "%i/%i/%i %i/%i/%i %i/%i/%i",
						&vertexIndices[0], &textureIndices[0], &normalIndices[0],
						&vertexIndices[0 + 1], &textureIndices[0 + 1], &normalIndices[0 + 1],
						&vertexIndices[0 + 2], &textureIndices[0 + 2], &normalIndices[0 + 2]);
				}
				else {
					sscanf_s(line.c_str(), "%i//%i %i//%i %i//%i",
						&vertexIndices[0], &normalIndices[0],
						&vertexIndices[0 + 1], &normalIndices[0 + 1],
						&vertexIndices[0 + 2], &normalIndices[0 + 2]);
				}

				// Indices are offset by 1
				for (unsigned int i = 0; i < 3; i++) {
					vertexIndices[i] -= 1;
					normalIndices[i] -= 1;
					textureIndices[i] -= 1;
					unsigned int index = indexer.getIndex(vertexBuffer[3 * vertexIndices[i]], vertexBuffer[3 * vertexIndices[i] + 1], vertexBuffer[3 * vertexIndices[i] + 2],
						normalBuffer[3 * normalIndices[i]], normalBuffer[3 * normalIndices[i] + 1], normalBuffer[3 * normalIndices[i] + 2],
						textureBuffer[2 * textureIndices[i]], textureBuffer[2 * textureIndices[i] + 1]);
					finalVertexIndices.push_back(index);
				}

				glm::vec3 v0, v1, v2;
				glm::vec2 uv0, uv1, uv2;
				v0 = glm::vec3(indexer.vertices[3* finalVertexIndices[0]], indexer.vertices[3* finalVertexIndices[0] + 1], indexer.vertices[3* finalVertexIndices[0] + 2]);
				v1 = glm::vec3(indexer.vertices[3* finalVertexIndices[0+1]], indexer.vertices[3* finalVertexIndices[0+1] + 1], indexer.vertices[3* finalVertexIndices[0+1] + 2]);
				v2 = glm::vec3(indexer.vertices[3* finalVertexIndices[0+2]], indexer.vertices[3* finalVertexIndices[0+2] + 1], indexer.vertices[3* finalVertexIndices[0+2] + 2]);
				uv0 = glm::vec2(indexer.textures[2 * (finalVertexIndices[0])], indexer.textures[2 * (finalVertexIndices[0]) + 1]);
				uv1 = glm::vec2(indexer.textures[2 * (finalVertexIndices[0+1])], indexer.textures[2 * (finalVertexIndices[0+1]) + 1]);
				uv2 = glm::vec2(indexer.textures[2 * (finalVertexIndices[0+2])], indexer.textures[2 * (finalVertexIndices[0+2]) + 1]);
				glm::vec3 deltaPos1 = v1 - v0;
				glm::vec3 deltaPos2 = v2 - v0;
				glm::vec2 deltaUV1 = uv1 - uv0;
				glm::vec2 deltaUV2 = uv2 - uv0;
				float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
				glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;

				for (unsigned int i = 0; i < 3; i++) {
					tangentBuffer.push_back(tangent.x);
					tangentBuffer.push_back(tangent.y);
					tangentBuffer.push_back(tangent.z);
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
		glGenBuffers(1, &this->textID);
		glBindBuffer(GL_ARRAY_BUFFER, this->textID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * indexer.textures.size(), &indexer.textures[0], GL_STATIC_DRAW);

		//Copy Tangents to GPU
		glGenBuffers(1, &this->tangentID);
		glBindBuffer(GL_ARRAY_BUFFER, this->tangentID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tangentBuffer.size(), &tangentBuffer[0], GL_STATIC_DRAW);

		//Set up the VAO
		glGenVertexArrays(1, &vaoID);
		glBindVertexArray(vaoID);
		//Bind Vertices
		glBindBuffer(GL_ARRAY_BUFFER, this->verticesID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		//Bind Normals
		glBindBuffer(GL_ARRAY_BUFFER, this->normalsID);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		//Bind textures
		glBindBuffer(GL_ARRAY_BUFFER, this->textID);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		//Bind tangents
		glBindBuffer(GL_ARRAY_BUFFER, this->tangentID);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		if (currentMtl != "" && indices.size() < inserted) {
			// Copy indices since the last one wont be copied.
			this->indices.push_back(0);
			this->faces.push_back(finalVertexIndices.size());
			glGenBuffers(1, &this->indices[this->indices.size() - 1]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indices[this->indices.size() - 1]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * finalVertexIndices.size(), &finalVertexIndices[0], GL_STATIC_DRAW);
		}

		this->name = name;
		objFile.close();
	}
	else {
		throw runtime_error("Unable to load obj file: " + path + ".obj");
	}
}

void NormalMappedMesh::draw(GLuint shaderID) {
	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	for (unsigned int i = 0; i < this->indices.size(); i++) {
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->mats[i]->textID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->mats[i]->normalID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indices[i]);
		glDrawElements(GL_TRIANGLES, this->faces[i], GL_UNSIGNED_INT, (void*)0);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

NormalMappedMesh::~NormalMappedMesh() {
	glDeleteBuffers(1, &this->verticesID);
	glDeleteBuffers(1, &this->normalsID);
	glDeleteBuffers(1, &this->textID);
	glDeleteVertexArrays(1, &vaoID);
	for (unsigned int i = 0; i < this->indices.size(); i++) {
		glDeleteBuffers(1, &this->indices[i]);
		delete mats[i];
	}
}