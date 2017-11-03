#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>

#include "Material.h"
#include "Model.h"

using namespace std;

class Mesh : public Model {
	public:
		GLuint verticesID, textID, normalsID, vaoID;
		vector<GLuint> indices;
		vector<unsigned int> faces;
		vector<Material*> mats;
		string path;
		bool textures;
	public:
		Mesh(string path);
		void draw(GLuint shaderID);
		virtual ~Mesh();
};