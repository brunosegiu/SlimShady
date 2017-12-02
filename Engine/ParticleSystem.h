#pragma once

#include <vector>
#include "Particle.h"
#include "FreeMesh.h"

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

	std::vector<Particle> particles;
	glm::vec3 gravity_point;
	FreeMesh* mesh;

public:
	//construct system given n number of particles
	ParticleSystem(float lenght, int minIVel, int maxIVel, int maxVel, int minMass, int maxMass, int force, int maxN);
	
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

	~ParticleSystem(void);
};

