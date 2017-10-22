#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>

#include "Material.h"
#include "Entity.h"

using namespace std;

class NormalMappedMesh : public Entity {
public:
	GLuint verticesID, textID, normalsID, tangentID, vaoID;
	vector<GLuint> indices;
	vector<unsigned int> faces;
	vector<Material*> mats;
public:
	NormalMappedMesh(string path);
	void draw(GLuint shaderID);
	~NormalMappedMesh();
private:
	void push(std::vector<float> &v, int cant);
	void push(std::vector<unsigned int> &v, int cant);
};