#include "OBJIndexer.h"

OBJIndexer::OBJIndexer() {

}

unsigned int OBJIndexer::getIndex(float v_0, float v_1, float v_2, float n_0, float n_1, float n_2, float t_0, float t_1) {
	/*for (unsigned int i = 0; i < vertices.size(); i+=3) {
		unsigned int ti = 2 * i / 3;
		if (vertices[i] == v_0 && vertices[i + 1] == v_1 && vertices[i + 2] == v_2 && normals[i] == n_0 && normals[i + 1] == n_1 && normals[i + 2] == n_2 && textures[ti] == t_0 && textures[ti+1] == t_1) {
			return i/3;
		}
	}*/
	vertices.push_back(v_0);
	vertices.push_back(v_1);
	vertices.push_back(v_2);
	normals.push_back(n_0);
	normals.push_back(n_1);
	normals.push_back(n_2);
	textures.push_back(t_0);
	textures.push_back(t_1);
	return (vertices.size() - 1)  / 3;
}