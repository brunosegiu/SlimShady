#include "ParticleSystem.h"
#include <math.h>
#include <GL/glew.h>

ParticleSystem::ParticleSystem(float lenght, int minIVel, int maxIVel, int maxVel, int minMass, int maxMass, int force, int maxN, glm::vec3 camPos)
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
	this->camPos = camPos;

	this->gravity_point = glm::vec3(0.0, -1.0, 0.0);

	//if(nParticles > MAX_PARTICLES)
		//n = MAX_PARTICLES;

	for(int i = 0; i < nParticles; i++)
	{
		Particle temp = Particle(lenght, minInitVelocity, maxInitVelocity, minMass, maxMass);
		particles.push_back(temp);
		index.push_back(i);
	}

	std::vector<Particle>::iterator it;
	std::vector<float> positions;			//Hago todas las cuentas de las particulas y despues pido las posiciones, eso es lo q en definitiva dibujo
	for (it = particles.begin(); it != particles.end(); it++) {
		positions.push_back(it->getPos().x);
		positions.push_back(it->getPos().y);
		positions.push_back(it->getPos().z);
	}

	this->mesh = new FreeMesh(positions,index);

	this->shader = new ShaderProgram();
	shader->loadShader("assets/shaders/rain.vert", GL_VERTEX_SHADER);
	shader->loadShader("assets/shaders/rain.geo",GL_GEOMETRY_SHADER);
	shader->loadShader("assets/shaders/rain.frag", GL_FRAGMENT_SHADER);
	if (!shader->loadProgram()) {
		printf("Unable to load basic shader!\n");
	}

	this->timestamp = clock();
	firstDraw = true;
}

	
//Function to advance state of particle system by time t in ms
void ParticleSystem::advance(float time)
{
	if (!firstDraw) {
		clock_t temp = clock();
		float elapsed = ((temp - this->timestamp) / double(CLOCKS_PER_SEC))/5;
		std::vector<float> positions;
		std::vector<Particle>::iterator it;
		for (it = particles.begin(); it != particles.end(); it++)
		{
			glm::vec3 force = gravity_point * (float)forceMagnitude;
			it->advance(elapsed, force, maxVelocity, lenght);
			positions.push_back(it->getPos().x);
			positions.push_back(it->getPos().y);
			positions.push_back(it->getPos().z);
		}
		this->mesh = new FreeMesh(positions, index);
		this->timestamp = temp;
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
	if (firstDraw) { firstDraw = false; this->timestamp = clock(); }
	this->shader->bind();
	GLuint worldTransformID = glGetUniformLocation(shader->getId(), "worldTransform");
	glm::mat4 toWorldCoords = mvp;
	glUniformMatrix4fv(worldTransformID, 1, GL_FALSE, &toWorldCoords[0][0]);
	this->mesh->draw(GL_POINTS);
}

void ParticleSystem::traslate(glm::vec3 newPos) {
	if (this->camPos != newPos) {
		glm::vec3 d = newPos - camPos;
		std::vector<Particle>::iterator it;
		for (it = particles.begin(); it != particles.end(); it++)
		{
			it->traslate(d, lenght);
		}
		this->camPos = newPos;
	}
}


ParticleSystem::~ParticleSystem()
{
}
