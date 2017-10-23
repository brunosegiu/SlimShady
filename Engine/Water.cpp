#include "Water.h"
#include <FreeImage.h>

Water::Water(){
	/*
	Usando perlin noise
	float maxHeight = 20.0f;
	FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename("assets/textures/heightmap.png");
	FIBITMAP* bitmap = FreeImage_Load(fif, "assets/textures/heightmap.png");
	RGBQUAD color;
	int width = FreeImage_GetHeight(bitmap);
	int height = FreeImage_GetWidth(bitmap);
	std::vector<float> positions;
	std::vector<unsigned int> index;
	for (unsigned int i = 0; i <= width; i++) {
		for (unsigned int j = 0; j <= height; j++) {
			positions.push_back((float)-(height / 2) + (j));
			FreeImage_GetPixelColor(bitmap, i, j, &color);
			float r = color.rgbRed / 255.0f;
			positions.push_back((r - 0.5f) * maxHeight);
			positions.push_back((float)-(width / 2) + (i));
		}
	}
	for (unsigned int i = 0; i <= width - 1; i++) {
		for (unsigned int j = 0; j <= height - 1; j++) {
			index.push_back(i * (height + 1) + j);
			index.push_back(i * (height + 1) + j + 1);
			index.push_back(i * (height + 1) + j + (height + 1) + 1);

			index.push_back(i * (height + 1) + j + (height + 1) + 1);
			index.push_back(i * (height + 1) + j + (height + 1));
			index.push_back(i * (height + 1) + j);
		}
	}
	this->mesh = new FreeMesh(positions, index);
	FreeImage_Unload(bitmap);


	*/

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