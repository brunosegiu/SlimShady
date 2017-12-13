#include "Character.h"

#include <glm/gtx/transform.hpp>

Character::Character(Model_Anim* model, vector<glm::vec3> waypoints, float speed, World* world, float scale) {
	this->pos = waypoints[0];
	this->waypoints = waypoints;
	this->speed = speed;
	this->model = model;
	this->currWayPoint = 0;
	this->world = world;
	this->shader = new ShaderProgram("assets/shaders/anim.vert", "assets/shaders/anim.frag");
	this->scale = glm::scale(glm::vec3(scale,scale,scale));
}

void Character::draw(float elapsed) {
	glm::vec3 dir = glm::normalize(waypoints[(currWayPoint + 1) % waypoints.size()] - waypoints[currWayPoint]);
	pos += dir * speed * elapsed;
	if (abs(glm::distance(pos, waypoints[currWayPoint]) + glm::distance(pos, waypoints[(currWayPoint + 1) % waypoints.size()]) - glm::distance(waypoints[currWayPoint], waypoints[(currWayPoint + 1) % waypoints.size()])) > 0.001f) {
		currWayPoint = (currWayPoint + 1) % waypoints.size();
	}
	shader->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model->textID);
	GLuint texID = glGetUniformLocation(shader->getId(), "sampler");
	glUniform1i(texID, 0);
	GLuint shaderID = this->shader->getId();

	GLuint lightDirID = glGetUniformLocation(shaderID, "lightdir");
	GLuint lightColorID = glGetUniformLocation(shaderID, "lightcolor");

	GLuint moonDirID = glGetUniformLocation(shaderID, "moondir");
	GLuint moonColorID = glGetUniformLocation(shaderID, "mooncolor");

	glUniform3fv(lightDirID, 1, &world->sun->light->dir[0]);
	glUniform3fv(lightColorID, 1, &(world->sun->light->color * world->sun->intensity)[0]);
	
	glUniform3fv(moonDirID, 1, &world->sun->moon->dir[0]);
	glUniform3fv(moonColorID, 1, &(world->sun->moon->color * world->sun->mIntensity)[0]);
	glUniformMatrix4fv(glGetUniformLocation(shader->getId(), "worldTransform"), 1, GL_FALSE, &(this->world->cam->modelViewProjectionMatrix * glm::translate(pos) * scale * glm::rotate(glm::radians(-90.0f), glm::vec3(0.0f,1.0f,0.0f)) * glm::rotate(atan2f(dir.z,dir.y), glm::vec3(0,1.0f,0)) )[0][0]);
	model->update(elapsed);
	model->draw(glGetUniformLocation(shader->getId(), "bones"));
}