#include "Terrain.h"

Terrain::Terrain(string heightmapPath) : Entity(){
	vector<float> positions;
	vector<unsigned int> index;
	for (unsigned int i = 0; i <= 10; i++) {  //n = 100
		for (unsigned int j = 0; j <= 10; j++) {
			positions.push_back((float)-5 + j); // n/2 = 50
			positions.push_back(0.0f);
			positions.push_back((float)-5 + i);
		}
	}
	for (unsigned int i = 0; i <= 9; i++) {
		for (unsigned int j = 0; j <= 9; j++) { //n-1 = 99
			index.push_back(i * 11 + j);
			index.push_back(i * 11 + j + 1); //n+1 = 101
			index.push_back(i * 11 + j + 11 + 1);

			index.push_back(i * 11 + j + 11 + 1);
			index.push_back(i * 11 + j + 11);
			index.push_back(i * 11 + j);
		}
	}
	this->grid = new FreeMesh(positions, index);
}

void Terrain::draw(GLuint shaderID) {
	this->grid->draw(0);
}

Terrain::~Terrain() {
	delete this->grid;
}
