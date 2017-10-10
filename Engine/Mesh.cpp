#include "Mesh.h"

#ifdef EXPERIMENTAL_MESH
#include <exception>
#include <iostream>
#include<fstream>
#include<string>
#include<iterator>

#include <glm/gtc/type_ptr.hpp>

vector<Material> loadMtl(string path) {
	string mtlExt = path + ".mtl";
	string line;
	ifstream mtlFile(mtlExt.c_str());
	vector<Material> mats;
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
					mats.push_back(Material(glm::vec3(ambient1, ambient2, ambient3), glm::vec3(diffuse1, diffuse2, diffuse3), glm::vec3(specular1,specular2, specular3)));
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
			mats.push_back(Material(glm::vec3(ambient1, ambient2, ambient3), glm::vec3(diffuse1, diffuse2, diffuse3), glm::vec3(specular1, specular2, specular3)));
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

				for (int j = 0; j < 3; j++) {
					vertexIndices[vertexIndex+j] -= 1.0f;
					normalIndices[vertexIndex +j] -= 1.0f;
					textureIndices[vertexIndex +j] -= 1.0f;
				}
				vertexIndex += 3;
			}
			else if (!strncmp(line.c_str(), prefix.c_str(), strlen(prefix.c_str()))) {
				inserted++;
				// No more vertices, normals or texture coords to load, gen arrays on GPU
				if (currentMtl == "") {
					// Copy vertices to GPU
					glGenVertexArrays(1, &handlerID);
					glBindVertexArray(handlerID);

					glGenBuffers(1, &this->verticesID);
					glBindBuffer(GL_ARRAY_BUFFER, this->verticesID);
					glBufferData(GL_ARRAY_BUFFER, sizeof(float) * connectedVert, vertexBuffer, GL_STATIC_DRAW);
					/*
					//Copy normals to GPU
					glGenBuffers(1, &this->normalsID);
					glBindBuffer(GL_ARRAY_BUFFER, this->normalsID);
					glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normalsCount, &normalBuffer[0], GL_STATIC_DRAW);

					//Copy UV to GPU
					if (textureCoords > 0) {
						glGenBuffers(1, &this->textID);
						glBindBuffer(GL_ARRAY_BUFFER, this->textID);
						glBufferData(GL_ARRAY_BUFFER, sizeof(float) * textureCoords, &textureBuffer[0], GL_STATIC_DRAW);
					}*/
				}
				else {
					// Copy indices
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

		if (currentMtl != "" && indices.size() < inserted) {
			// Copy indices
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

		delete[] vertexIndices;
		delete[] normalIndices;
		delete[] textureIndices;
	}
	else {
		throw runtime_error("Unable to load obj file: " + path + ".obj");
	}
}

void Mesh::draw() {
	glBindVertexArray(handlerID);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->verticesID);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
	for (unsigned int i = 0; i < this->indices.size(); i++) {
		/*glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(this->mats[i].ambient) );
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(this->mats[i].diffuse));
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(this->mats[i].specular));*/
		glColor3f(1.0f, 1.0f, 1.0f);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indices[i]);
		glDrawElements(GL_TRIANGLES,this->faces[i],	GL_UNSIGNED_INT,(void*)0);
	}
	glDisableVertexAttribArray(0);
}

Mesh::~Mesh() {
	glDeleteBuffers(1, &this->verticesID);
	glDeleteBuffers(1, &this->normalsID);
	glDeleteBuffers(1, &this->textID);
	for (unsigned int i = 0; i < this->indices.size(); i++) {
		glDeleteBuffers(1, &this->indices[i]);
	}
}
#endif
#ifndef EXPERIMENTAL_MESH
#include "Mesh.h"
#include <iostream>

using namespace std;

Mesh::Mesh() {
}

void Mesh::load(string objName) {
	string objExt = objName + ".obj";
	loadMtl(objName);
	string line;
	ifstream objFile(objExt.c_str());
	if (objFile.is_open()) {
		objFile.seekg(0, ios::end);
		long fileSize = objFile.tellg();
		objFile.seekg(0, ios::beg);

		string currentMtl = "";

		float* vertexBuffer = new float[fileSize];
		float* textureBuffer = new float[fileSize];
		float* normalBuffer = new float[fileSize];

		int triangleIndex = 0;
		int normalIndex = 0;
		int textureCoordIndex = 0;

		int connectedVert = 0;
		int textureCoords = 0;
		int normalsCount = 0;

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
				int vertexNumber[3] = { 0,0,0 };
				int normalNumber[3] = { 0,0,0 };
				int textureNumber[3] = { 0,0,0 };
				if (textureCoords > 0) {
					sscanf_s(line.c_str(), "%i/%i/%i %i/%i/%i %i/%i/%i",
						&vertexNumber[0], &textureNumber[0], &normalNumber[0],
						&vertexNumber[1], &textureNumber[1], &normalNumber[1],
						&vertexNumber[2], &textureNumber[2], &normalNumber[2]);
				}
				else {
					sscanf_s(line.c_str(), "%i//%i %i//%i %i//%i",
						&vertexNumber[0], &normalNumber[0],
						&vertexNumber[1], &normalNumber[1],
						&vertexNumber[2], &normalNumber[2]);
				}

				for (int j = 0; j < 3; j++) {
					vertexNumber[j] -= 1;
					normalNumber[j] -= 1;
					textureNumber[j] -= 1;
				}

				for (int i = 0; i < 3; i++) {
					faces[currentMtl][triangleIndex] = vertexBuffer[3 * vertexNumber[i]];
					faces[currentMtl][triangleIndex + 1] = vertexBuffer[3 * vertexNumber[i] + 1];
					faces[currentMtl][triangleIndex + 2] = vertexBuffer[3 * vertexNumber[i] + 2];
					triangleIndex += 3;

					normals[currentMtl][normalIndex] = normalBuffer[3 * normalNumber[i]];
					normals[currentMtl][normalIndex + 1] = normalBuffer[3 * normalNumber[i] + 1];
					normals[currentMtl][normalIndex + 2] = normalBuffer[3 * normalNumber[i] + 2];
					normalIndex += 3;

					if (textureCoords > 0) {
						textures[currentMtl][textureCoordIndex] = textureBuffer[2 * textureNumber[i]];
						textures[currentMtl][textureCoordIndex + 1] = textureBuffer[2 * textureNumber[i] + 1];
						textureCoordIndex += 2;
					}
				}
				faceVertices[currentMtl] += 9;
			}
			else if (!strncmp(line.c_str(), prefix.c_str(), strlen(prefix.c_str()))) {
				currentMtl = line.substr(7, line.length());
				faces[currentMtl] = new float[fileSize];
				normals[currentMtl] = new float[fileSize];
				textures[currentMtl] = new float[fileSize];
				faceVertices[currentMtl] = 0;
				triangleIndex = 0;
				normalIndex = 0;
				textureCoordIndex = 0;
			}

		}
		objFile.close();
		delete[] vertexBuffer;
		delete[] textureBuffer;
		delete[] normalBuffer;
	}
	else {
		printf("NO SE PUDO ABRIR EL ARCHIVO");
	}
}

void Mesh::loadMtl(string objName) {
	string mtlExt = objName + ".mtl";
	string line;
	ifstream mtlFile(mtlExt.c_str());
	if (mtlFile.is_open()) {
		string currentMtl = "";

		string prefixMTL = "newmtl";
		string prefixKa = "Ka";
		string prefixKd = "Kd";
		string prefixKs = "Ks";
		string prefixNs = "Ns";
		string prefixMapKd = "map_Kd";

		float ambient1, ambient2, ambient3 = 0;
		float diffuse1, diffuse2, diffuse3 = 0;
		float specular1, specular2, specular3 = 0;
		float aux1, aux2, aux3;
		float shininess = 0;
		string texturePath = "";

		while (!mtlFile.eof()) {
			getline(mtlFile, line);
			if (!strncmp(line.c_str(), prefixMTL.c_str(), strlen(prefixMTL.c_str()))) {
				currentMtl = line.substr(7, line.length());
				textureId[currentMtl] = -1;
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
			else if (!strncmp(line.c_str(), prefixNs.c_str(), strlen(prefixNs.c_str()))) {
				string data = line.substr(strlen(prefixNs.c_str()) - 1, strlen(line.c_str())).c_str();
				data[0] = data[1] = ' ';
				sscanf_s(data.c_str(), "%f ", &aux1);
				shininess = aux1;
			}
			else if (!strncmp(line.c_str(), prefixKs.c_str(), strlen(prefixKs.c_str()))) {
				string data = line.substr(strlen(prefixKs.c_str()) - 1, strlen(line.c_str())).c_str();
				sscanf_s(data.c_str(), "%f %f %f", &aux1, &aux2, &aux3);
				specular[currentMtl] = new float[4];
				specular1 = aux1;
				specular2 = aux2;
				specular3 = aux3;
			}
			else if (!strncmp(line.c_str(), prefixMapKd.c_str(), strlen(prefixMapKd.c_str()))) {
				string texturePath = line.substr(strlen(prefixMapKd.c_str()) + 1, strlen(line.c_str())).c_str();
				/*if (textureId[currentMtl] == -1){
				//Cargamos la textura en RAM
				FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(data.c_str()); //Se obtiene formato de la textura
				FIBITMAP* bitmap = FreeImage_Load(fif, data.c_str()); //se crea un bitmap a partir de la textura
				bitmap = FreeImage_ConvertTo24Bits(bitmap); //se convierte a 24 bits para coincidir con el formato RGB
				//Pasamos la textura a OpenGL
				glGenTextures(1, &textureId[currentMtl]); //Asifgna un valor de identificador a textureId
				glBindTexture(GL_TEXTURE_2D, textureId[currentMtl]); // Asignamos el identificador a la textura
				int h = FreeImage_GetHeight(bitmap);
				int w = FreeImage_GetWidth(bitmap);
				void* pixels = FreeImage_GetBits(bitmap); //VOID???
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels);
				delete[] pixels;
				}*/
			}
		}
		mtlFile.close();
		//return Material(glm::vec3(ambient1, ambient2, ambient3, 1.0f), glm::vec3(diffuse1, diffuse2, diffuse3, 1.0f), glm::vec3(specular1, specular2, specular3, 1.0f));
	}
	else {
		printf("NO SE PUDO ABRIR EL ARCHIVO");
	}
}

Mesh::~Mesh() {
	for (map<string, GLuint>::iterator it = textureId.begin(); it != textureId.end(); it++) {
		if (it->second != -1)
			glDeleteTextures(1, &it->second);
	}

	for (map<string, float*>::iterator it = faces.begin(); it != faces.end(); it++) {
		//delete[] it->second;
	}

	for (map<string, float*>::iterator it = normals.begin(); it != normals.end(); it++) {
		//delete[] it->second;
	}

	for (map<string, float*>::iterator it = textures.begin(); it != textures.end(); it++) {
//		delete[] it->second;
	}

	for (map<string, float*>::iterator it = specular.begin(); it != specular.end(); it++) {
//		delete[] it->second;
	}

	for (map<string, float*>::iterator it = diffuse.begin(); it != diffuse.end(); it++) {
	//	delete[] it->second;
	}

	for (map<string, float*>::iterator it = ambient.begin(); it != ambient.end(); it++) {
	//	delete[] it->second;
	}
}

void Mesh::draw() {
	glPushMatrix();
	for (map<string, float*>::iterator it = faces.begin(); it != faces.end(); it++) {
		string mtl = it->first;
		bool texturesEnabled = false;
		if (faceVertices[mtl] >= 3) {
			glColor3f(1, 1, 1);
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, faces[mtl]);
			glDrawArrays(GL_TRIANGLES, 0, faceVertices[mtl] - faceVertices[mtl] % 3);
			glDisableClientState(GL_VERTEX_ARRAY);
		}
	}
	glPopMatrix();
}
#endif
