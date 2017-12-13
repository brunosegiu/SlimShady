#pragma once

#include <glm/glm.hpp>

#include "ShaderProgram.h"
#include "Model_Anim.h"
#include "World.h"


class World;
class Character {
	public:
		Model_Anim* model;
		vector<glm::vec3> waypoints;
		glm::vec3 pos;
		float speed;
		int currWayPoint;
		ShaderProgram* shader;
		World* world;
		glm::mat4 scale;
		
		Character(Model_Anim* model, vector<glm::vec3> waypoints, float speed, World* world, float scale);
		void draw(float elapsed);

};