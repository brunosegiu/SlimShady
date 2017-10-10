#pragma once

#define EXPERIMENTAL_MESH
#ifdef EXPERIMENTAL_MESH
#include <string>
#include <vector>

#include <GL/glew.h>

#include "Material.h"

using namespace std;

class Mesh {
	public:
		GLuint verticesID, textID, normalsID, handlerID;
		vector<GLuint> indices;
		vector<GLuint> faces;
		vector<Material> mats;
	public:
		Mesh(string path);
		void draw();
		~Mesh();
};
#endif
#ifndef EXPERIMENTAL_MESH
#include <windows.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <SDL_opengl.h>
#include <SDL.h>
#include <map>
#include <vector>

using namespace std;
class Mesh {
public:
	void load(string objName);
	Mesh();
	void draw();
	virtual ~Mesh();

private:
	map<string, float*> faces;
	map<string, unsigned long> faceVertices;

	map<string, float*> normals;

	map<string, float*> textures;

	map<string, float*> specular;
	map<string, float*> diffuse;
	map<string, float*> ambient;
	map<string, float> shininess;
	map<string, GLuint> textureId;
	void loadMtl(string objName);
};
#endif
