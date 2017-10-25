#pragma once

#include <vector>

#include<GL/glew.h>

using namespace std;

class OBJIndexer {
	public:
		vector<float> vertices;
		vector<float> normals;
		vector<float> textures;
		vector<float> tangent;
	public:
		OBJIndexer();
		unsigned int getIndex(float v_0, float v_1, float v_2, float n_0, float n_1,float n_2, float t_0,float t_1);
};