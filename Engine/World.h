#pragma once

#include <vector>
#include <ctime>
#include <string>
#include <map>

#include "Camera.h"
#include "ShaderProgram.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Terrain.h"
#include "Water.h"
#include "Sun.h"
#include "Skybox.h"
#include "Entity.h"

using namespace std;

class Entity;

class World {
	public:
		// Entidades
		std::vector<Entity*> meshes;
		std::vector<Entity*> meshes_nm;
		std::vector<Entity*> meshes_free;
		std::vector<Entity*> meshes_inst;
		std::map<string, Model*> models;
		Entity* terrain;
		Entity* water;
		//Skybox* sky;
		Camera* cam;
		

		//Luces
		Sun* sun;
		std::vector<DirectionalLight*> dirLights;
		
		//Shaders
		ShaderProgram* basic;
		ShaderProgram* basicNM;
		ShaderProgram* basicInst;
		ShaderProgram* veryBasic;

		clock_t lastDraw;

	public:
		World(Camera* cam);
		void draw();
		void save(string path);
		static World* load(string path, SDL_Window* win, float width, float height);
		void addModel(Model* model);
		void addEntity(string model);
		~World();
};