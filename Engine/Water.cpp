#include "Water.h"

Water::Water(){
	std::vector<float> positions;
	std::vector<unsigned int> index;
	for (float i = 0; i <= 100; i+=0.5) {  //n = 100
		for (float j = 0; j <= 100; j+=0.5) {
			positions.push_back((float)-50 + (j)); // n/2 = 50
			positions.push_back(0.0f);
			positions.push_back((float)-50 + (i));
		}
	}
	for (unsigned int i = 0; i <= 199; i++) {
		for (unsigned int j = 0; j <= 199; j++) { //n-1 = 99
			index.push_back(i * 201 + j);
			index.push_back(i * 201 + j + 1); //n+1 = 101
			index.push_back(i * 201 + j + 201 + 1);

			index.push_back(i * 201 + j + 201 + 1);
			index.push_back(i * 201 + j + 201);
			index.push_back(i * 201 + j);
		}
	}
	this->mesh = new FreeMesh(positions, index);
}

Water::~Water() {
	delete mesh;
}