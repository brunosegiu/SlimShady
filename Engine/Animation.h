#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/vec3.hpp>

#include "Joint.h"

using namespace std;

class Animation {
	public:
		Animation(string path);
		void draw(GLuint shaderID);

		Joint* root;
		vector<glm::mat4> currentTransforms;
		float elapsed;
		int jointNumber;
		//GPU
		GLuint vaoID, indicesID, faces;
};
