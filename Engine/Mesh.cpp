#include "Mesh.h"

#include <exception>
#include <iostream>
#include<fstream>
#include<string>
#include<iterator>

#include <glm/gtc/type_ptr.hpp>

vector<Material*> loadMtl(string path) {
	string mtlExt = path + ".mtl";
	string line;
	ifstream mtlFile(mtlExt.c_str());
	vector<Material*> mats;
	if (mtlFile.is_open()) {

		unsigned int inserted = 0;
		string currentMtl = "";
		string prefixMTL = "newmtl";
		string prefixKa = "Ka";
		string prefixKd = "Kd";
		string prefixKs = "Ks";
		string prefixNs = "Ns";
		string prefixMapKd = "map_Kd";

		float aux1, aux2, aux3;
		float ambient1, ambient2, ambient3 = 0;
		float diffuse1, diffuse2, diffuse3 = 0;
		float specular1, specular2, specular3 = 0;
		string texturePath = "";

		while (!mtlFile.eof()) {
			getline(mtlFile, line);
			if (!strncmp(line.c_str(), prefixMTL.c_str(), strlen(prefixMTL.c_str()))) {
				inserted++;
				if (currentMtl != "") {
					Material* mat = new Material(glm::vec3(ambient1, ambient2, ambient3), glm::vec3(diffuse1, diffuse2, diffuse3), glm::vec3(specular1, specular2, specular3), texturePath);
					mats.push_back(mat);
				}
				currentMtl = line.substr(7, line.length());
			}
			else if (!strncmp(line.c_str(), prefixKa.c_str(), strlen(prefixKa.c_str()))) {
				string data = line.substr(strlen(prefixKa.c_str()) - 1, strlen(line.c_str()));
				data[0] = data[1] = ' ';
				sscanf_s(data.c_str(), "%f %f %f", &aux1, &aux2, &aux3);
				ambient1 = aux1;
				ambient2 = aux2;
				ambient3 = aux3;
			}
			else if (!strncmp(line.c_str(), prefixKd.c_str(), strlen(prefixKd.c_str()))) {
				string data = line.substr(strlen(prefixKd.c_str()) - 1, strlen(line.c_str())).c_str();
				data[0] = data[1] = ' ';
				sscanf_s(data.c_str(), "%f %f %f", &aux1, &aux2, &aux3);
				diffuse1 = aux1;
				diffuse2 = aux2;
				diffuse3 = aux3;
			}
			else if (!strncmp(line.c_str(), prefixKs.c_str(), strlen(prefixKs.c_str()))) {
				string data = line.substr(strlen(prefixKs.c_str()) - 1, strlen(line.c_str())).c_str();
				sscanf_s(data.c_str(), "%f %f %f", &aux1, &aux2, &aux3);
				specular1 = aux1;
				specular2 = aux2;
				specular3 = aux3;
			}
			else if (!strncmp(line.c_str(), prefixMapKd.c_str(), strlen(prefixMapKd.c_str()))) {
				texturePath = line.substr(strlen(prefixMapKd.c_str()) + 1, strlen(line.c_str())).c_str();
			}
		}

		if (currentMtl != "" && mats.size() < inserted) {
			mats.push_back( new Material(glm::vec3(ambient1, ambient2, ambient3), glm::vec3(diffuse1, diffuse2, diffuse3), glm::vec3(specular1, specular2, specular3), texturePath));
		}

		mtlFile.close();
		return mats;
	}
	else {
		throw runtime_error("Unable to load mtl file: " + path + ".mtl");
	}
}

Mesh::Mesh(string path) {
	float* normals;
	float* faces;
	float* textures;
	string fullpath = path + ".obj";
	this->mats = loadMtl(path);
	string line;
	ifstream objFile(fullpath.c_str());
	if (objFile.is_open()) {
		objFile.seekg(0, ios::end);
		long fileSize = long(objFile.tellg());
		objFile.seekg(0, ios::beg);

		unsigned int inserted = 0;

		string currentMtl = "";

		float* vertexBuffer = new float[fileSize];
		float* textureBuffer = new float[fileSize];
		float* normalBuffer = new float[fileSize];
		float* finalNormalBuffer = new float[fileSize];
		float* finalTextureBuffer = new float[fileSize];

		unsigned int* vertexIndices = new unsigned int[fileSize];
		unsigned int* normalIndices = new unsigned int[fileSize];
		unsigned int* textureIndices = new unsigned int[fileSize];

		int connectedVert = 0;
		int textureCoords = 0;
		int normalsCount = 0;

		int vertexIndex = 0;

		string prefix = "usemtl";

		while (!objFile.eof()) {
			getline(objFile, line);
			if (line.c_str()[0] == 'v' && line.c_str()[1] == ' ') {
				line[0] = ' ';
				sscanf_s(line.c_str(), "%f %f %f ",
					&vertexBuffer[connectedVert],
					&vertexBuffer[connectedVert + 1],
					&vertexBuffer[connectedVert + 2]);
				connectedVert += 3;
			}
			else if (line.c_str()[0] == 'v' && line.c_str()[1] == 't') {
				line[0] = ' ';
				line[1] = ' ';
				sscanf_s(line.c_str(), "%f %f ",
					&textureBuffer[textureCoords],
					&textureBuffer[textureCoords + 1]);
				textureCoords += 2;
			}
			else if (line.c_str()[0] == 'v' && line.c_str()[1] == 'n') {
				line[0] = ' ';
				line[1] = ' ';
				sscanf_s(line.c_str(), "%f %f %f ",
					&normalBuffer[normalsCount],
					&normalBuffer[normalsCount + 1],
					&normalBuffer[normalsCount + 2]);
				normalsCount += 3;
			}
			else if (line.c_str()[0] == 'f') {
				line[0] = ' ';
				
				if (textureCoords > 0) {
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

				// Since OpenGL doesn't support normal/texture indirection, set a normal per vertex (and a text coord)
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						finalNormalBuffer[3 * (vertexIndices[vertexIndex+i]) + j] = normalBuffer[3 * normalIndices[vertexIndex + i] + j];
					}
				}
				if (textureCoords > 0) {
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
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * vertexIndex, vertexIndices, GL_STATIC_DRAW);
				}
				currentMtl = line.substr(7, line.length());
				vertexIndex = 0;
			}
		}

		//Copy vertices to GPU
		glGenBuffers(1, &this->verticesID);
		glBindBuffer(GL_ARRAY_BUFFER, this->verticesID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * connectedVert, vertexBuffer, GL_STATIC_DRAW);

		//Copy normals to GPU
		glGenBuffers(1, &this->normalsID);
		glBindBuffer(GL_ARRAY_BUFFER, this->normalsID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * connectedVert, finalNormalBuffer, GL_STATIC_DRAW);

		//Copy UV to GPU
		if (textureCoords > 0) {
			glGenBuffers(1, &this->textID);
			glBindBuffer(GL_ARRAY_BUFFER, this->textID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * (connectedVert / 3), finalTextureBuffer, GL_STATIC_DRAW);
		}
		
		if (currentMtl != "" && indices.size() < inserted) {
			// Copy indices since the last one wont be copied.
			this->indices.push_back(0);
			this->faces.push_back(vertexIndex);
			glGenBuffers(1, &this->indices[this->indices.size() - 1]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indices[this->indices.size() - 1]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * vertexIndex, vertexIndices, GL_STATIC_DRAW);
		}

		objFile.close();
		delete[] vertexBuffer;
		delete[] textureBuffer;
		delete[] normalBuffer;
		delete[] finalTextureBuffer;
		delete[] finalNormalBuffer;

		delete[] vertexIndices;
		delete[] normalIndices;
		delete[] textureIndices;

		this->textures = textureCoords > 0;
	}
	else {
		throw runtime_error("Unable to load obj file: " + path + ".obj");
	}
}

void Mesh::draw() {
	glBindBuffer(GL_ARRAY_BUFFER, this->verticesID);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalsID);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, (void*)0);
	if (this->textures) {
		glBindBuffer(GL_ARRAY_BUFFER, this->textID);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, (void*)0);
	}
	for (unsigned int i = 0; i < this->indices.size(); i++) {
		if (this->mats[i]->isTextured) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, this->mats[i]->textID);
		}
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &mats[i]->ambient.x);
		GLfloat dif[] = {1.0f,1.0f,1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &mats[i]->specular.x);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indices[i]);
		glDrawElements(GL_TRIANGLES,this->faces[i],	GL_UNSIGNED_INT,(void*)0);
		if (this->mats[i]->isTextured) {
			glDisable(GL_TEXTURE_2D);
		}
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	if (this->textures)
		glDisableClientState(GL_VERTEX_ARRAY);
}

Mesh::~Mesh() {
	glDeleteBuffers(1, &this->verticesID);
	glDeleteBuffers(1, &this->normalsID);
	if (this->textures)
		glDeleteBuffers(1, &this->textID);
	for (unsigned int i = 0; i < this->indices.size(); i++) {
		glDeleteBuffers(1, &this->indices[i]);
		delete mats[i];
	}
}