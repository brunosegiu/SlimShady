#pragma once

#include <GL/glew.h>
#include <FreeImage.h>

class TerrainTile {
	public:
		TerrainTile(int width, int height, int offsetx, int offsety, float maxHeight, FIBITMAP* bitmap);
		void draw();

		unsigned int vertexCount, indexCount;
		GLuint verticesID, normalsID, indexID, vaoID;
};