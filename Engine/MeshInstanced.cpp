#include "MeshInstanced.h"
#include "tinyxml2.h"
#include <iostream>
using namespace std;
using namespace tinyxml2;

MeshInstanced::MeshInstanced(string modelPath, vector<float> coords)
	: Mesh(modelPath) {
	this->instancesCoords = coords;
	this->instancesCount = coords.size() % 3;
	//Copy instance coordinates to GPU
	glGenBuffers(1, &this->instancesCoordID);
	glBindBuffer(GL_ARRAY_BUFFER, this->instancesCoordID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*instancesCoords.size(), &instancesCoords[0], GL_STREAM_DRAW);
	//GL_STREAM_DRAW "The data store contents will be modified once and used at most a few times"
	//Es decir, serán modificados algunas veces y se usaran algunas veces

	//Set up the VAO
	glBindVertexArray(vaoID);
	//Tercera posicion, ya que texturas es la segunda en caso de tener
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//Unbind things
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void MeshInstanced::drawInstanced(GLuint shaderID) {
	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(3);
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
		glUniform3f(kd, this->mats[i]->diffuse.x, this->mats[i]->diffuse.y, this->mats[i]->diffuse.z);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indices[i]);
		glDrawElementsInstanced(GL_TRIANGLES, this->faces[i], GL_UNSIGNED_INT, (void*)0, instancesCount);
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
	glDisableVertexAttribArray(3);
	if (textures)
		glDisableVertexAttribArray(2);

}
MeshInstanced::MeshInstanced(string modelPath, string coordsPath)
	:Mesh(modelPath) {
	/*
	Lee las coordenadas para la posición en el mundo donde se quiere instanciar al mesh
	El formato XML es el siguiente:

	<Model>
	<Name>MODEL_NAME</Name>
	<PrimCount>INSTANCES_COUNT</PrimCount>
	<Coordinates>
	<Position x="0" y="0" z="0"/>
	<Position x="10" y="0" z="0"/>
	<Position x="0" y="0" z="10"/>
	...
	...
	<Position x="10" y="0" z="10"/>
	</Coordinates>
	</Model>
	*/
	string name;
	int primCount;
	//Inicializacion
	instancesCoords = std::vector<float>();

	string coordsExt = coordsPath + ".xml";
	XMLDocument xmlDoc;
	xmlDoc.LoadFile(coordsExt.c_str());

	//Navegacion al primer nodo
	XMLNode* root = xmlDoc.FirstChild();
	//Acceso a los atributos de los hijos
	XMLElement* pName = root->FirstChildElement("Name");
	name = string(pName->GetText());
	//Cómo se obtiene un int
	XMLElement* pPrimCount = root->FirstChildElement("PrimCount");
	pPrimCount->QueryIntText(&primCount);
	this->instancesCount = primCount;

	XMLElement* pCoordinates = root->FirstChildElement("Coordinates");

	for (XMLElement* pPositions = pCoordinates->FirstChildElement("Position"); pPositions != NULL; pPositions = pPositions->NextSiblingElement("Position")) {
		float x, y, z;
		pPositions->QueryFloatAttribute("x", &x);
		pPositions->QueryFloatAttribute("y", &y);
		pPositions->QueryFloatAttribute("z", &z);

		instancesCoords.push_back(x);
		instancesCoords.push_back(y);
		instancesCoords.push_back(z);
	}

	//Copy instance coordinates to GPU
	glGenBuffers(1, &this->instancesCoordID);
	glBindBuffer(GL_ARRAY_BUFFER, this->instancesCoordID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*instancesCoords.size(), &instancesCoords[0], GL_STREAM_DRAW);

	//Set up the VAO
	glBindVertexArray(vaoID);
	//Tercera posicion, ya que texturas es la segunda en caso de haberlas
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//Indica que el atributo 3 es un atributo per-instance cada instancia (1)
	glVertexAttribDivisor(3, 1);

	//Unbind things
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

MeshInstanced::~MeshInstanced() {
	this->instancesCoords.clear();
	glDeleteBuffers(1, &this->verticesID);
}