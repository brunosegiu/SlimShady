#pragma once

#include "Entity.h"
#include "Camera.h"
#include "ShaderProgram.h"

class World {
	public:
		vector<Entity*> worldEntities;
		Camera* cam;
		
		ShaderProgram* basic;

		World(Camera* cam);
		void draw();
		~World();
};