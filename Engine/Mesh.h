#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>

#include "Material.h"

using namespace std;

class Mesh {
	public:
		GLuint verticesID, textID, normalsID;
		vector<GLuint> indices;
		vector<unsigned int> faces;
		vector<Material*> mats;
		bool textures;
	public:
		Mesh(string path);
		void draw();
		~Mesh();
};