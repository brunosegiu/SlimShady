#pragma once

#include <vector>
#include <ctime>
#include <string>
#include <map>
#include <utility>

#include "Camera.h"
#include "ShaderProgram.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Terrain.h"
#include "Water.h"
#include "Sun.h"
#include "Skybox.h"
#include "Entity.h"
#include "Filter.h"

using namespace std;

class Entity;
class Skybox;

class World {
	public:
		// Entidades
		std::vector<Entity*> meshes;
		std::vector<Entity*> meshes_nm;
		std::vector<Entity*> meshes_free;
		std::vector<Entity*> meshes_inst;
		vector<Entity*> terrains;
		std::map<string, Model*> models;
		Entity* water;
		Skybox* sky;
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

		vector<pair<bool,Filter*>> filters;

		float gamma, contrast, brightness;

	public:
		World(Camera* cam);
		void draw();
		void save(string path);
		static World* load(string path, SDL_Window* win, float width, float height);
		void addModel(Model* model);
		void addEntity(string model);
		~World();
};