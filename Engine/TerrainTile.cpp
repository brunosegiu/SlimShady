#include "TerrainTile.h"

#include <vector>

#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;

float getHeight(FIBITMAP* bitmap, int i, int j, int width, int height) {
	if (i >= 0 && i < width && j >= 0 && j <= height) {
		RGBQUAD color;
		FreeImage_GetPixelColor(bitmap, i, j, &color);
		return 2 * ((color.rgbRed / 255.0f) - 0.5f);
	}
	else {
		return 0;
	}
}

void addNormal(vector<float> &normals, int i, int j, int width, int height, FIBITMAP* bitmap) {
	int offset = 20;
	float heightL = getHeight(bitmap, i - offset, j, width, height);
	float heightR = getHeight(bitmap, i + offset, j, width, height);
	float heightD = getHeight(bitmap, i, j - offset, width, height);
	float heightU = getHeight(bitmap, i, j + offset, width, height);
	glm::vec3 normal = glm::vec3(heightL - heightR, 1.0f, heightD - heightU);
	normal = normalize(normal);
	normals.push_back(normal.x);
	normals.push_back(normal.y);
	normals.push_back(normal.z);
}

TerrainTile::TerrainTile(int width, int height, int offsetx, int offsety, float maxHeight, FIBITMAP* bitmap) {
	int heightHeightMap = FreeImage_GetHeight(bitmap);
	int widthHeightMap = FreeImage_GetWidth(bitmap);
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<unsigned int> index;
	for (int i = 0; i <= width; i++) {
		for (int j = 0; j <= height; j++) {
			positions.push_back(float(offsety) + float(j));
			float normalizedHeight = getHeight(bitmap, i+offsetx, j+offsety, widthHeightMap, heightHeightMap);
			positions.push_back((normalizedHeight)* maxHeight);
			positions.push_back(float(offsetx) + float(i));
			addNormal(normals, i+offsetx, j+offsety, widthHeightMap, heightHeightMap, bitmap);
		}
	}
	for (unsigned int i = 0; i <= width - 1; i++) {
		for (unsigned int j = 0; j <= height - 1; j++) {
			index.push_back(i * (height + 1) + j + (height + 1) + 1);
			index.push_back(i * (height + 1) + j + 1);
			index.push_back(i * (height + 1) + j);

			index.push_back(i * (height + 1) + j);
			index.push_back(i * (height + 1) + j + (height + 1));
			index.push_back(i * (height + 1) + j + (height + 1) + 1);
		}
	}

	vertexCount = positions.size() / 3;
	indexCount = index.size();

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glGenBuffers(1, &indexID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(unsigned int), &index[0], GL_STATIC_DRAW);

	glGenBuffers(1, &verticesID);
	glBindBuffer(GL_ARRAY_BUFFER, this->verticesID);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), &positions[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &this->normalsID);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalsID);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void TerrainTile::draw() {
	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
