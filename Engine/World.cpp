#include "World.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <ctime>

World::World(Camera* cam) {
	this->cam = cam;
	//Shaders
	this->basic = new ShaderProgram("assets/shaders/basic.vert", "assets/shaders/basic.frag");
	this->veryBasic = new ShaderProgram();
	this->veryBasic->loadShader("assets/shaders/sinusoide.vert", GL_VERTEX_SHADER);
	this->veryBasic->loadShader("assets/shaders/sinusoide.geo", GL_GEOMETRY_SHADER);
	this->veryBasic->loadShader("assets/shaders/veryBasic.frag", GL_FRAGMENT_SHADER);
	this->veryBasic->loadProgram();
	this->geomertyPassShader = new ShaderProgram("assets/shaders/geometryPass.vert", "assets/shaders/geometryPass.frag");
	this->gbuf = new GBuffer(cam->width, cam->height);
	this->basicNM = new ShaderProgram("assets/shaders/basic_normalmap.vert", "assets/shaders/basic_normalmap.frag");
}

void World::draw() {
	this->cam->update();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (meshes.size() > 0){
		//Render meshes
		this->basic->bind();
		GLuint worldTransformID = glGetUniformLocation(basic->getId(), "worldTransform");
		GLuint textID = glGetUniformLocation(basic->getId(), "textSampler");
		GLuint modelTransformID = glGetUniformLocation(basic->getId(), "modelTransform");
		glUniform1i(textID, 0);
		for (unsigned int i = 0; i < meshes.size(); i++) {
			//this->worldEntities[i]->rotate(0.04f, glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 toWorldCoords = this->cam->modelViewProjectionMatrix * this->meshes[i]->modelMatrix;
			glm::mat4 modelTransf = this->meshes[i]->modelMatrix;
			glUniformMatrix4fv(modelTransformID, 1, GL_FALSE, &modelTransf[0][0]);
			glUniformMatrix4fv(worldTransformID, 1, GL_FALSE, &toWorldCoords[0][0]);
			this->meshes[i]->draw(basic->getId());
		}
		this->basic->unbind();
	}
	if (meshes_nm.size() > 0){
		//Render normal mapped meshes
		this->basicNM->bind();
		GLuint worldTransformID = glGetUniformLocation(basicNM->getId(), "worldTransform");
		GLuint textID = glGetUniformLocation(basicNM->getId(), "textSampler");
		GLuint normTextID = glGetUniformLocation(basicNM->getId(), "normTextSampler");
		GLuint modelTransformID = glGetUniformLocation(basicNM->getId(), "modelTransform");
		GLuint modelViewID = glGetUniformLocation(basicNM->getId(), "modelView");
		glUniform1i(textID, 0);
		glUniform1i(normTextID, 1);

		for (unsigned int i = 0; i < meshes_nm.size(); i++) {
			glm::mat4 toWorldCoords = this->cam->modelViewProjectionMatrix * this->meshes_nm[i]->modelMatrix;
			glm::mat4 modelTransf = this->meshes_nm[i]->modelMatrix;
			glUniformMatrix3fv(modelViewID, 1, GL_FALSE, &glm::mat3(this->cam->viewMatrix * this->meshes_nm[i]->modelMatrix)[0][0]);
			glUniformMatrix4fv(modelTransformID, 1, GL_FALSE, &modelTransf[0][0]);
			glUniformMatrix4fv(worldTransformID, 1, GL_FALSE, &toWorldCoords[0][0]);
			this->meshes_nm[i]->draw(basic->getId());
		}
		this->basicNM->unbind();
	}

	this->veryBasic->bind();
	worldTransformID = glGetUniformLocation(veryBasic->getId(), "worldTransform");
	GLuint phiID = glGetUniformLocation(veryBasic->getId(), "phi");
	glUniform1f(phiID, time);
	//printf("%f\n", time);
	for (unsigned int j = 0; j < meshes.size(); j++) {
		glm::mat4 toWorldCoords = this->cam->modelViewProjectionMatrix;
		glUniformMatrix4fv(worldTransformID, 1, GL_FALSE, &toWorldCoords[0][0]);
		this->meshes[j]->draw();
	}
}

World::~World() {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		delete this->meshes[i];
	}
	for (unsigned int i = 0; i < meshes_nm.size(); i++) {
		delete this->meshes_nm[i];
	}
	for (unsigned int i = 0; i < dirLights.size(); i++) {
		delete this->dirLights[i];
	}
	delete cam;
	delete basic;
}