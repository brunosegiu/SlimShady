#pragma once

#include <glm/glm.hpp>
#include <stdlib.h>
#include <ctime>

class Particle
{	
	float mass;
	glm::vec3 velocity;
	glm::vec3 position;
	int minIVel,maxIVel,minMass,maxMass;

public:
	Particle(int lenght, int minIVel, int maxIVel, int minMass, int maxMass);

	float rand_float()
	{
		float value = rand() / float(RAND_MAX);
		return value;
	}

	//Function to advance state of particle by time t in ms and force in given direction
	void advance(float, glm::vec3, int, int);

	void traslate(glm::vec3 distance, float lenght);

	//Function to get position
	glm::vec3 getPos();

	//Function to draw a particle
	//void draw(int);

	~Particle();
};

