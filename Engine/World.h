#pragma once

#include <vector>
#include <ctime>
#include <string>

#include "Entity.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "FreeMesh.h"
#include "Terrain.h"

using namespace std;

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

		clock_t lastDraw;

	public:
		World(Camera* cam);
		World();
		void draw();
		void save(string path);
		static World* load(string path, SDL_Window* win, float width, float height);
		~World();
};