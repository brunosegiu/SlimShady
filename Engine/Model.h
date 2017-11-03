#pragma once

#include <string>

#include <GL/glew.h>

class Model {
public:
	Model();
	virtual void draw(GLuint shaderID) = 0;
	~Model();

	std::string name;
};
