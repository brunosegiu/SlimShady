#pragma once

#include <string>

#include <GL/glew.h>

#include "FreeMesh.h"
#include "ShaderProgram.h"
#include "TerrainTile.h"

using namespace std;

class Terrain : public Model{
	public:
		Terrain(string heightmapPath, float maxHeight, int tilesX, int tilesY);
		~Terrain();
		void draw(GLuint shaderID);

		ShaderProgram* terrainShader;
		vector<TerrainTile*> tiles;
		GLuint textureID;
		string path;
		float maxHeight;
		int tilesX, tilesY;
		glm::mat4 viewProjectionMatrix;
		
};
