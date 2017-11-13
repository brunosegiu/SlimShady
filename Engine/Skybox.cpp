#include "Skybox.h"


Skybox::Skybox(int size) {
	unsigned int arrI[] = {
		0,1,2,
		2,3,0 };
	std::vector<unsigned int> index(arrI, arrI + sizeof(arrI) / sizeof(arrI[0]));
	float arrT[] = {
		0,0,
		1,0,
		1,1,
		0,1};
	std::vector<float> textcoords(arrT, arrT + sizeof(arrT) / sizeof(arrT[0]));

	float arrF[] = {
		-size/2, -size/2, size/2,
		size/2, -size/2, size/2,
		size/2, size / 2, size/2,
		-size/2, size/2, size/2};
	std::vector<float> front (arrF, arrF + sizeof(arrF) / sizeof(arrF[0]) );
	this->front = new FreeMesh(front,index);
	this->front->addTexture(textcoords);
	this->frontID = loadTexture("assets/textures/bluecloud_ft.jpg");

	float arrU[] = {
		size / 2, size / 2, -size / 2,
		size / 2, size / 2, size / 2,
		-size / 2, size / 2, size / 2,
		-size / 2, size / 2, -size / 2};
	std::vector<float> up (arrU, arrU + sizeof(arrU) / sizeof(arrU[0]));
	this->up = new FreeMesh(up, index);
	this->up->addTexture(textcoords);
	this->upID = loadTexture("assets/textures/bluecloud_dn.jpg");

	float arrR[] = {
		size / 2, -size / 2, size / 2,
		size / 2, -size / 2, -size / 2,
		size / 2, size / 2, -size / 2,
		size / 2, size / 2, size / 2 };
	std::vector<float> right(arrR, arrR + sizeof(arrR) / sizeof(arrR[0]));
	this->right = new FreeMesh(right, index);
	this->right->addTexture(textcoords);
	this->rightID = loadTexture("assets/textures/bluecloud_lf.jpg");

	float arrB[] = {
		size / 2, size / 2, -size / 2,
		-size / 2, size / 2, -size / 2,
		-size / 2, -size / 2, -size / 2,
		size / 2, -size / 2, -size / 2};
	std::vector<float> back(arrB, arrB + sizeof(arrB) / sizeof(arrB[0]));
	this->back = new FreeMesh(back, index);
	this->back->addTexture(textcoords);
	this->backID = loadTexture("assets/textures/bluecloud_bk.jpg");

	float arrD[] = {
		-size / 2, -size / 2, -size / 2,
		size / 2, -size / 2, -size / 2,
		size / 2, -size / 2, size / 2,
		-size / 2, -size / 2, size / 2 };
	std::vector<float> down(arrD, arrD + sizeof(arrD) / sizeof(arrD[0]));
	this->down = new FreeMesh(down, index);
	this->down->addTexture(textcoords);
	this->downID = loadTexture("assets/textures/bluecloud_up.jpg");

	float arrL[] = {
		-size / 2, size / 2, -size / 2,
		-size / 2, size / 2, size / 2,
		-size / 2, -size / 2, size / 2,
		-size / 2, -size / 2, -size / 2};
	std::vector<float> left(arrL, arrL + sizeof(arrL) / sizeof(arrL[0]));
	this->left = new FreeMesh(left, index);
	this->left->addTexture(textcoords);
	this->leftID = loadTexture("assets/textures/bluecloud_rt.jpg");
	this->shader = new ShaderProgram("assets/shaders/skybox.vert", "assets/shaders/skybox.frag");
}

GLuint Skybox::loadTexture(std::string path) {
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

void Skybox::draw(GLuint coso) {
	glDisable(GL_CULL_FACE);
	this->shader->bind();
	GLuint worldTransformID = glGetUniformLocation(shader->getId(), "worldTransform");
	//GLuint phiID = glGetUniformLocation(shader->getId(), "phi");
	//GLuint camID = glGetUniformLocation(shader->getId(), "cameraPos");
	//GLuint lightDirID = glGetUniformLocation(shader->getId(), "lightdir");
	GLuint lightColorID = glGetUniformLocation(shader->getId(), "lightcolor");
	GLuint intensityID = glGetUniformLocation(shader->getId(), "intensity");
	//GLuint moonDirID = glGetUniformLocation(shader->getId(), "moondir");
	GLuint moonColorID = glGetUniformLocation(shader->getId(), "mooncolor");
	GLuint mintensityID = glGetUniformLocation(shader->getId(), "mintensity");
	//glUniform1f(phiID, this->lastDraw / double(CLOCKS_PER_SEC));
	glm::mat4 toWorldCoords = mvp /** this->mesh->modelMatrix*/;
	//glm::vec3 camDir = this->camPos;
	glUniformMatrix4fv(worldTransformID, 1, GL_FALSE, &toWorldCoords[0][0]);
	//glUniform3fv(camID, 1, &camDir[0]);
	//glUniform3fv(lightDirID, 1, &this->lightDir[0]);
	glUniform3fv(lightColorID, 1, &this->lightColor[0]);
	glUniform1f(intensityID, this->intensity);
	//glUniform3fv(moonDirID, 1, &this->moonDir[0]);
	glUniform3fv(moonColorID, 1, &this->moonColor[0]);
	glUniform1f(mintensityID, this->mIntensity);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->frontID);
	GLuint textID = glGetUniformLocation(shader->getId(), "textSampler");
	glUniform1i(textID, 0);
	this->front->draw(0);
	glBindTexture(GL_TEXTURE_2D, this->upID);
	this->up->draw(0);
	glBindTexture(GL_TEXTURE_2D, this->rightID);
	this->right->draw(0);
	glBindTexture(GL_TEXTURE_2D, this->backID);
	this->back->draw(0);
	glBindTexture(GL_TEXTURE_2D, this->downID);
	this->down->draw(0);
	glBindTexture(GL_TEXTURE_2D, this->leftID);
	this->left->draw(0);
	glEnable(GL_CULL_FACE);
}

Skybox::~Skybox() {
	delete front;
	delete up;
	delete right;
	delete back;
	delete down;
	delete left;
}