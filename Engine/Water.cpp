#include "Water.h"

Water::Water(){
	std::vector<float> positions;
	std::vector<unsigned int> index;
	for (unsigned int i = 0; i <= 100; i++) {  //n = 100
		for (unsigned int j = 0; j <= 100; j++) {
			positions.push_back((float)-50 + (j)); // n/2 = 50
			positions.push_back(0.0f);
			positions.push_back((float)-50 + (i));
		}
	}
	for (unsigned int i = 0; i <= 99; i++) {
		for (unsigned int j = 0; j <= 99; j++) { //n-1 = 99
			index.push_back(i * 101 + j);
			index.push_back(i * 101 + j + 1); //n+1 = 101
			index.push_back(i * 101 + j + 101 + 1);

			index.push_back(i * 101 + j + 101 + 1);
			index.push_back(i * 101 + j + 101);
			index.push_back(i * 101 + j);
		}
	}
	this->mesh = FreeMesh(positions, index);
}

Water::~Water() {

}