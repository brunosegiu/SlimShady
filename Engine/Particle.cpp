#include "Particle.h"
#include <math.h>

Particle::Particle(int lenght, int minIVel, int maxIVel, int minMass, int maxMass) {
	this->mass = rand() % (maxMass - minMass) + minMass;
	this->velocity = glm::vec3(0, -1, 0) * (float)( minIVel + rand() % (maxIVel - minIVel));
	this->position = glm::vec3((1 - 2 * rand_float())*lenght, (1 - 2 * rand_float())*lenght, (1 - 2 * rand_float())*lenght);
	this->minIVel = minIVel; this->maxIVel = maxIVel; this->minMass = minMass; this->maxMass = maxMass;
}

//function to advance state by time t in ms
void Particle::advance(float t, glm::vec3 force, int maxVelocity, int lenght)
{
	//calculating acceleration
	glm::vec3 acc = force / mass;

	//calculating velocity
	velocity = velocity + acc*(float)(t);
	
	if(sqrt(glm::dot(velocity, velocity)) >= maxVelocity)
		velocity = normalize(velocity) * (float)maxVelocity;

	//changing position
	position = position+velocity*(float)(t);

	if (position.y <= -lenght) {
		float remainder = (float)fmod(abs(position.y), lenght);
		position.y = lenght - remainder;
		//this->mass = rand() % (maxMass - minMass) + minMass;
		this->velocity = glm::vec3(0, -1, 0) * (float)(minIVel + rand() % (maxIVel - minIVel));
	}

}


Particle::~Particle(){
}

//Function to get position
glm::vec3 Particle :: getPos()
{
	return position;
}

/*/Function to draw a particle
void particle :: draw(int trail_size)
{
	vec3d temp = (force - position).unit();
	if(temp.x < 0)
		temp.x *= -1;
	if(temp.y < 0)
		temp.y *= -1;
	if(temp.z < 0)
		temp.z *= -1;
	
}*/