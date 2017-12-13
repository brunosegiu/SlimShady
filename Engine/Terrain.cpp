#include "Terrain.h"

#include <FreeImage.h>
#include <glm/vec3.hpp>
#include <glm\gtx\transform.hpp>

#include "Material.h"

Terrain::Terrain(string heightmapPath, float maxHeight, int tilesX, int tilesY){
	this->name = "Terrain";
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
	this->blendMapID = Material::loadTexture("assets/textures/blendmap.png");
	this->texture1ID = Material::loadTexture("assets/textures/sand.jpg");
	this->texture2ID = Material::loadTexture("assets/textures/rock.jpg");
	this->texture3ID = Material::loadTexture("assets/textures/path.png");
	this->texture4ID = Material::loadTexture("assets/textures/grass.jpg");
}

void Terrain::draw(GLuint shaderID) {
	this->terrainShader->bind();
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->blendMapID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->texture1ID);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->texture2ID);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, this->texture3ID);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, this->texture4ID);
	GLuint blendID = glGetUniformLocation(shaderID, "blendMapSampler");
	glUniform1i(blendID, 0);
	GLuint text1ID = glGetUniformLocation(shaderID, "texture1Sampler");
	glUniform1i(text1ID, 1);
	GLuint text2ID = glGetUniformLocation(shaderID, "texture2Sampler");
	glUniform1i(text2ID, 2);
	GLuint text3ID = glGetUniformLocation(shaderID, "texture3Sampler");
	glUniform1i(text3ID, 3);
	GLuint text4ID = glGetUniformLocation(shaderID, "texture4Sampler");
	glUniform1i(text4ID, 4);

	glBindTexture(GL_TEXTURE_2D, this->texture4ID);
	for (unsigned int i = 0; i < tiles.size(); i++) {
		tiles[i]->draw();
	}
}

std::vector<float> Terrain::grassPositions() {
	std::vector<float> retPositions;
	FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename("assets/textures/blendmap.png");
	FIBITMAP* bitmap = FreeImage_Load(fif, "assets/textures/blendmap.png");

	for (int i = 0; i < tiles.size(); i++) {
		TerrainTile* iter = tiles.at(i);
		for (int j = 0; j < iter->positions.size()-2; j+=3) {
			float colorF;
			RGBQUAD color;
			FreeImage_GetPixelColor(bitmap, iter->losI.at(j/3), iter->losJ.at(j/3), &color);
			if (color.rgbRed == 0 && color.rgbGreen == 0 && color.rgbBlue == 0) {
				retPositions.push_back(iter->positions.at(j));
				retPositions.push_back(iter->positions.at(j + 1));
				retPositions.push_back(iter->positions.at(j + 2));
			}
		}
	}
	return retPositions;
}

Terrain::~Terrain() {
	delete this->terrainShader;
	for (unsigned int i = 0; i < this->tiles.size(); i++) {
		delete tiles[i];
	}
	glDeleteTextures(1, &this->blendMapID);
	glDeleteTextures(1, &this->texture1ID);
	glDeleteTextures(1, &this->texture2ID);
	glDeleteTextures(1, &this->texture3ID);
	glDeleteTextures(1, &this->texture4ID);
}
