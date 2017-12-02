//#include <Windows.h>
//#include <gl/glut.h>

#include "Particle.h"

Particle::Particle(int lenght, int minIVel, int maxIVel, int minMass, int maxMass) {
	this->mass = rand() % (maxMass - minMass) + minMass;
	this->velocity = glm::vec3(rand_float(), rand_float(), rand_float()) * (float)( minIVel + rand() % (maxIVel - minIVel));
	this->position = glm::vec3((1 - 2 * rand_float())*lenght, (1 - 2 * rand_float())*lenght, (1 - 2 * rand_float())*lenght);
}

//function to advance state by time t in ms
void Particle::advance(float t, glm::vec3 force, int maxVelocity, int lenght)
{
	//calculating acceleration
	glm::vec3 acc = force / mass;

	//calculating velocity
	velocity = velocity + acc*(float)(t/1000.0);
	
	if(sqrt(glm::dot(velocity, velocity)) >= maxVelocity)
		velocity = normalize(velocity) * (float)maxVelocity;

	//changing position
	position = position+velocity*(float)(t/1000.0);

	if(position.x <= -lenght)
		position.x = lenght;
	else if(position.x >= lenght)
		position.x = -lenght;

	if(position.y <= -lenght)
		position.y = lenght;
	else if(position.y >= lenght)
		position.y = -lenght;

	if(position.z <= -lenght)
		position.z = lenght;
	else if(position.z >= lenght)
		position.z = -lenght;
}


Particle::~Particle(void){
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