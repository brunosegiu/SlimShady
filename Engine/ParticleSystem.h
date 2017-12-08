#pragma once

#include "Particle.h"
#include "FreeMesh.h"
#include "ShaderProgram.h"
#include <ctime>

class ParticleSystem
{

	int nParticles; //Cantidad de particulas con las que se inicia el sistema
	float lenght;
	int minInitVelocity;
	int maxInitVelocity;
	int maxVelocity;
	int minMass;
	int maxMass;
	int forceMagnitude;
	int maxParticles; //Cantidad maxima de particulas en el sistema en todo momento

	std::vector<Particle> particles; //cambiar esto por un vector de vectores con sus propios timestamp
	std::vector<unsigned int> index;
	clock_t timestamp;
	glm::vec3 gravity_point;
	FreeMesh* mesh;
	ShaderProgram* shader;
	bool firstDraw;
	glm::vec3 camPos;

public:
	glm::mat4 mvp;

	//construct system given n number of particles
	ParticleSystem(float lenght, int minIVel, int maxIVel, int maxVel, int minMass, int maxMass, int force, int maxN, glm::vec3 camPos);
	
	//Function to advance state of particle system by time t in ms
	void advance(float);

	//Function to set gravity point
	void setGravity(glm::vec3 gravity);

	//Function to add particles
	bool addParticles(int);

	//Function to delete particles
	bool deleteParticles(int);

	//Function to draw particles
	void draw();

	//Funciton to traslate particles
	void traslate(glm::vec3 newPos);

	~ParticleSystem();
};

