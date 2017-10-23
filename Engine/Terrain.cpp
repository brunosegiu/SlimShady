#include "Terrain.h"
#include <FreeImage.h>
#include <glm/vec3.hpp>
#include <glm\gtx\transform.hpp>

Terrain::Terrain(string heightmapPath, float maxHeight, int tilesX, int tilesY) : Entity(){
	FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(heightmapPath.c_str());
	FIBITMAP* bitmap = FreeImage_Load(fif, heightmapPath.c_str());
	int terrSizeY = FreeImage_GetHeight(bitmap);
	int terrSizeX = FreeImage_GetWidth(bitmap);
	int tileSizeX = terrSizeX / tilesX;
	int tileSizeY = terrSizeY / tilesY;
	for (int i = 0; i < tilesX; i++) {
		for (int j = 0; j < tilesY; j++) {
			int offsetx = i*tileSizeX;
			int offsety = j*tileSizeY;
			TerrainTile* tile = new TerrainTile(tileSizeX, tileSizeY, offsetx, offsety, maxHeight, bitmap);
			this->tiles.push_back(tile);
		}
	}
	FreeImage_Unload(bitmap);
}

void Terrain::draw(GLuint shaderID) {
	for (unsigned int i = 0; i < tiles.size(); i++) {
		tiles[i]->draw();
	}
}

Terrain::~Terrain() {
}
