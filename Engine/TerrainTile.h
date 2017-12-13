#pragma once

#include <GL/glew.h>
#include <FreeImage.h>
#include <vector>

class TerrainTile {
	public:
		TerrainTile(int width, int height, int offsetx, int offsety, float maxHeight, FIBITMAP* bitmap);
		void draw();
public:
		std::vector<float> positions;
		std::vector<float> normals;
		std::vector<float> textCoords;
		std::vector<unsigned int> index;
		std::vector<unsigned int> losI;
		std::vector<unsigned int> losJ;

		unsigned int vertexCount, indexCount;
		GLuint verticesID, normalsID, textureID, indexID, vaoID;
};