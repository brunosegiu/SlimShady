#pragma once

#include "Entity.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "GBuffer.h"
#include "FreeMesh.h"

class World {
	public:
		// Entidades
		vector<Entity*> worldEntities;
		vector<FreeMesh*> meshes;
		Camera* cam;

		//Luces
		vector<DirectionalLight*> dirLights;
		
		//Shaders
		ShaderProgram* basic;
		ShaderProgram* veryBasic;

		//Deferred Shading
		ShaderProgram* geomertyPassShader;
		ShaderProgram* lightPassShader;
		GBuffer* gbuf;
	public:
		World(Camera* cam);
		void draw(); // Deferred shading
		void dummyDraw(float time); // Basic shading
		~World();
	private:
		void geomertyPass();
		void lightPass();
};