#include "Terrain.h"

#include <FreeImage.h>
#include <glm/vec3.hpp>
#include <glm\gtx\transform.hpp>

#include "Material.h"

Terrain::Terrain(string heightmapPath, float maxHeight, int tilesX, int tilesY) : Entity(){
	this->path = heightmapPath;
	this->maxHeight = maxHeight;
	this->tilesX = tilesX;
	this->tilesY = tilesY;
	this->terrainShader = new ShaderProgram("assets/shaders/terrain.vert", "assets/shaders/terrain.frag");
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
	this->textureID = Material::loadTexture("assets/textures/grass.jpg");
}

void Terrain::draw(GLuint shaderID) {
	this->terrainShader->bind();
	GLuint modelTransformID = glGetUniformLocation(terrainShader->getId(), "modelTransform");
	GLuint worldTransformID = glGetUniformLocation(terrainShader->getId(), "worldTransform");
	glUniformMatrix4fv(modelTransformID, 1, GL_FALSE, &this->modelMatrix[0][0]);
	glm::mat4 toWorldCoords = this->viewProjectionMatrix * this->modelMatrix;
	glUniformMatrix4fv(worldTransformID, 1, GL_FALSE, &toWorldCoords[0][0]);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
	GLuint textID = glGetUniformLocation(shaderID, "textSampler");
	glUniform1i(textID, 0);
	for (unsigned int i = 0; i < tiles.size(); i++) {
		tiles[i]->draw();
	}
}

Terrain::~Terrain() {
	delete this->terrainShader;
	for (unsigned int i = 0; i < this->tiles.size(); i++) {
		delete tiles[i];
	}
	glDeleteTextures(1, &this->textureID);
}
