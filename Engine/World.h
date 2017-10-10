#pragma once

#include "Mesh.h"

class World {
	public:
		vector<Mesh> meshes;
		World();
		void draw();
};