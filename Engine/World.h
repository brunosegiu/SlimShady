#pragma once

#include <vector>
#include <ctime>

#include "Entity.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "FreeMesh.h"
#include "Terrain.h"

class World {
	public:
		// Entidades
		std::vector<Entity*> meshes;
		std::vector<Entity*> meshes_nm;
		std::vector<Entity*> meshes_free;
		Terrain* terrain;
		Camera* cam;

		//Luces
		std::vector<DirectionalLight*> dirLights;
		
		//Shaders
		ShaderProgram* basic;
		ShaderProgram* basicNM;
		ShaderProgram* veryBasic;
		ShaderProgram* terrainShader;

		clock_t lastDraw;

	public:
		World(Camera* cam);
		void draw();
		~World();
};