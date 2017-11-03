#include "Water.h"
#include <FreeImage.h>

Water::Water(int width, int height){
	this->name = "Water";
	std::vector<float> positions;
	std::vector<unsigned int> index;
	std::vector<float> textcoords;
	for (float i = 0; i <= width; i++/*=0.5*/) {  //n = 100
		for (float j = 0; j <= height; j++/*=0.5*/) {
			positions.push_back((float)-height/2.0f + (j)); // n/2 = 50
			positions.push_back(0.0f);
			positions.push_back((float)-width/2.0f + (i));
			textcoords.push_back(i/50);
			textcoords.push_back(j/50);
		}
	}
	for (unsigned int i = 0; i <= /*2**/width -1; i++) {
		for (unsigned int j = 0; j <= /*2**/height -1; j++) { //n-1 = 99
			index.push_back(i * (/*2**/height+1) + j);
			index.push_back(i * (height + 1) + j + 1); //n+1 = 101
			index.push_back(i * (height + 1) + j + (height + 1) + 1);

			index.push_back(i * (height + 1) + j + (height + 1) + 1);
			index.push_back(i * (height + 1) + j + (height + 1));
			index.push_back(i * (height + 1) + j);
		}
	}
	this->mesh = new FreeMesh(positions, index);
	this->mesh->addTexture(textcoords);
	this->shader = new ShaderProgram("assets/shaders/water.vert", "assets/shaders/water.frag");

	this->textureID = loadTexture("assets/textures/deep_water3.jpg");
}

GLuint Water::loadTexture(std::string path) {
	GLuint id;
	FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(path.c_str());
	FIBITMAP* bitmap = FreeImage_Load(fif, path.c_str());
	bitmap = FreeImage_ConvertTo32Bits(bitmap);

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	int h = FreeImage_GetHeight(bitmap);
	int w = FreeImage_GetWidth(bitmap);
	void* pixels = FreeImage_GetBits(bitmap);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	FreeImage_Unload(bitmap);
	return id;
}

void Water::draw(GLuint shaderID) {
	glDisable(GL_CULL_FACE);
	this->shader->bind();
	GLuint worldTransformID = glGetUniformLocation(shader->getId(), "worldTransform");
	GLuint phiID = glGetUniformLocation(shader->getId(), "phi");
	GLuint camID = glGetUniformLocation(shader->getId(), "cameraPos");
	GLuint lightDirID = glGetUniformLocation(shader->getId(), "lightdir");
	GLuint lightColorID = glGetUniformLocation(shader->getId(), "lightcolor");
	glUniform1f(phiID, this->lastDraw / double(CLOCKS_PER_SEC));
	glm::mat4 toWorldCoords = mvp /** this->mesh->modelMatrix*/;
	glm::vec3 camDir = this->camPos;
	glUniformMatrix4fv(worldTransformID, 1, GL_FALSE, &toWorldCoords[0][0]);
	glUniform3fv(camID, 1, &camDir[0]);
	glUniform3fv(lightDirID, 1, &this->lightDir[0]);
	glUniform3fv(lightColorID, 1, &this->lightColor[0]);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
	GLuint textID = glGetUniformLocation(shader->getId(), "textSampler");
	glUniform1i(textID, 0);
	this->mesh->draw(0);
	glEnable(GL_CULL_FACE);
}

Water::~Water() {
	delete mesh;
	delete shader;
}