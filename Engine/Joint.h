#pragma once

#include <vector>
#include <string>

#include <glm/mat4x4.hpp>

using namespace std;

class Joint {
	public:
		Joint(string name, int pos);
		void addMatrices(vector<glm::mat4> &matrices);
		void calcBindMatrix(glm::mat4 &parent);

		vector<Joint*> children;
		string name;
		int pos;
		glm::mat4 transform;
};
