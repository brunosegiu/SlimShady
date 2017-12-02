#include "ParticleSystem.h"
//#include <GL/glut.h>
#include <math.h>

ParticleSystem::ParticleSystem(float lenght, int minIVel, int maxIVel, int maxVel, int minMass, int maxMass, int force, int maxN)
{
	this->nParticles = abs(maxN/2);
	this->lenght = lenght;
	this->minInitVelocity = minIVel;
	this->maxInitVelocity = maxIVel;
	this->maxVelocity = maxVel;
	this->minMass = minMass;
	this->maxMass = maxMass;
	this->forceMagnitude = force;
	this->maxParticles = maxN;

	//if(nParticles > MAX_PARTICLES)
		//n = MAX_PARTICLES;

	for(int i = 0; i < nParticles; i++)
	{
		Particle temp = Particle(lenght, minInitVelocity, maxInitVelocity, minMass, maxMass);
		particles.push_back(temp);
	}

	//this->mesh = new FreeMesh(particles,/*como son los indices de GL_POINTS?*/,/*Hay que agregar que se pueda elegir que tipo de figura quiero*/);
}

	
//Function to advance state of particle system by time t in ms
void ParticleSystem::advance(float time)
{
	std::vector<Particle>::iterator it;
	for(it = particles.begin(); it != particles.end(); it++)
	{
		glm::vec3 force = normalize(gravity_point - it->getPos()) * (float)forceMagnitude;
		it->advance(time, force, maxVelocity, lenght);
	}
}

//Function to set gravity point
void ParticleSystem::setGravity(glm::vec3 gravity){
	gravity_point = gravity;
}

//Function to add particles
bool ParticleSystem :: addParticles(int num)
{
	int i;
	for(i = 0; i < num && particles.size() < maxParticles; i++)
	{
		Particle p = Particle(lenght, minInitVelocity, maxInitVelocity, minMass, maxMass);
		particles.push_back(p);
	}
	return (i >= num);
}

//Function to delete particles(least is 0)
bool ParticleSystem :: deleteParticles(int num)
{
	int i;
	for(i = 0; i < num && particles.size() > 0; i++)
	{
		particles.pop_back();
	}

	return (i >= num);
}

//Function to draw a particle
void ParticleSystem::draw()
{
	std::vector<Particle>::iterator it;
	for(it = particles.begin(); it != particles.end(); it++){
		glm::vec3 pos = it->getPos();
		float k = sqrt(glm::dot(gravity_point-pos, gravity_point - pos))/(1.5*lenght);

		glBegin(GL_POINTS);
			glVertex3f(pos.x, pos.y, pos.z);
		glEnd();
	}
}


ParticleSystem::~ParticleSystem(void)
{
}
