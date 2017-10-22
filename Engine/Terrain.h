#pragma once

#include <string>

#include "FreeMesh.h"

using namespace std;

class Terrain : public Entity{
	public:
		Terrain(string heightmapPath);
		~Terrain();
		void draw(GLuint shaderID);
		FreeMesh* grid;
};
