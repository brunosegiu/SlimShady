#include "Water.h"
#include <FreeImage.h>

Water::Water(int width, int height){
	std::vector<float> positions;
	std::vector<unsigned int> index;
	for (float i = 0; i <= width; i+=0.5) {  //n = 100
		for (float j = 0; j <= height; j+=0.5) {
			positions.push_back((float)-height/2.0f + (j)); // n/2 = 50
			positions.push_back(0.0f);
			positions.push_back((float)-width/2.0f + (i));
		}
	}
	for (unsigned int i = 0; i <= 2*width -1; i++) {
		for (unsigned int j = 0; j <= 2*height -1; j++) { //n-1 = 99
			index.push_back(i * (2*height+1) + j);
			index.push_back(i * (2 * height + 1) + j + 1); //n+1 = 101
			index.push_back(i * (2 * height + 1) + j + (2 * height + 1) + 1);

			index.push_back(i * (2 * height + 1) + j + (2 * height + 1) + 1);
			index.push_back(i * (2 * height + 1) + j + (2 * height + 1));
			index.push_back(i * (2 * height + 1) + j);
		}
	}
	this->mesh = new FreeMesh(positions, index);
	this->shader = new ShaderProgram("assets/shaders/water.vert", "assets/shaders/water.frag");
}

void Water::draw(GLuint shaderID) {
	glDisable(GL_CULL_FACE);
	this->shader->bind();
	GLuint worldTransformID = glGetUniformLocation(shader->getId(), "worldTransform");
	GLuint phiID = glGetUniformLocation(shader->getId(), "phi");
	GLuint camID = glGetUniformLocation(shader->getId(), "cameraPos");
	GLuint lightDirID = glGetUniformLocation(shader->getId(), "lightdir");
	glUniform1f(phiID, this->lastDraw / double(CLOCKS_PER_SEC));
	glm::mat4 toWorldCoords = mvp * this->mesh->modelMatrix;
	glm::vec3 camDir = this->camPos;
	glUniformMatrix4fv(worldTransformID, 1, GL_FALSE, &toWorldCoords[0][0]);
	glUniform3fv(camID, 1, &camDir[0]);
	glUniform3fv(lightDirID, 1, &this->lightDir[0]);
	this->mesh->draw(0);
	glEnable(GL_CULL_FACE);
}

Water::~Water() {
	delete mesh;
	delete shader;
}