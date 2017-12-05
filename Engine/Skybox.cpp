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

	float halfdist = size / 2;
	float arrF[] = {
		-halfdist, -halfdist, halfdist,
		halfdist, -halfdist, halfdist,
		halfdist, halfdist, halfdist,
		-halfdist, halfdist, halfdist};
	std::vector<float> front (arrF, arrF + sizeof(arrF) / sizeof(arrF[0]) );
	this->front = new FreeMesh(front,index);
	this->front->addTexture(textcoords);
	this->frontID = loadTexture("assets/textures/bluecloud_ft.jpg");
	this->nfrontID = loadTexture("assets/textures/front.jpg");

	float arrU[] = {
		halfdist, halfdist, -halfdist,
		halfdist, halfdist, halfdist,
		-halfdist, halfdist, halfdist,
		-halfdist, halfdist, -halfdist};
	std::vector<float> up (arrU, arrU + sizeof(arrU) / sizeof(arrU[0]));
	this->up = new FreeMesh(up, index);
	this->up->addTexture(textcoords);
	this->upID = loadTexture("assets/textures/bluecloud_dn.jpg");
	this->nupID = loadTexture("assets/textures/up.jpg");

	float arrR[] = {
		halfdist, -halfdist, halfdist,
		halfdist, -halfdist, -halfdist,
		halfdist, halfdist, -halfdist,
		halfdist, halfdist, halfdist };
	std::vector<float> right(arrR, arrR + sizeof(arrR) / sizeof(arrR[0]));
	this->right = new FreeMesh(right, index);
	this->right->addTexture(textcoords);
	this->rightID = loadTexture("assets/textures/bluecloud_lf.jpg");
	this->nrightID = loadTexture("assets/textures/right.jpg");

	float arrB[] = {
		halfdist, halfdist, -halfdist,
		-halfdist, halfdist, -halfdist,
		-halfdist, -halfdist, -halfdist,
		halfdist, -halfdist, -halfdist};
	std::vector<float> back(arrB, arrB + sizeof(arrB) / sizeof(arrB[0]));
	this->back = new FreeMesh(back, index);
	this->back->addTexture(textcoords);
	this->backID = loadTexture("assets/textures/bluecloud_bk.jpg");
	this->nbackID = loadTexture("assets/textures/back.jpg");

	float arrD[] = {
		-halfdist, -halfdist, -halfdist,
		halfdist, -halfdist, -halfdist,
		halfdist, -halfdist, halfdist,
		-halfdist, -halfdist, halfdist };
	std::vector<float> down(arrD, arrD + sizeof(arrD) / sizeof(arrD[0]));
	this->down = new FreeMesh(down, index);
	this->down->addTexture(textcoords);
	this->downID = loadTexture("assets/textures/bluecloud_up.jpg");
	this->ndownID = loadTexture("assets/textures/down.jpg");

	float arrL[] = {
		-halfdist, halfdist, -halfdist,
		-halfdist, halfdist, halfdist,
		-halfdist, -halfdist, halfdist,
		-halfdist, -halfdist, -halfdist};
	std::vector<float> left(arrL, arrL + sizeof(arrL) / sizeof(arrL[0]));
	this->left = new FreeMesh(left, index);
	this->left2 = new FreeMesh(left, index);
	this->left->addTexture(textcoords);
	this->left2->addTexture(textcoords);
	this->leftID = loadTexture("assets/textures/bluecloud_rt.jpg");
	this->nleftID = loadTexture("assets/textures/left.jpg");

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
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	FreeImage_Unload(bitmap);
	return id;
}

void Skybox::draw(GLuint coso) {
	glDisable(GL_CULL_FACE);
	this->shader->bind();
	GLuint phiID = glGetUniformLocation(shader->getId(), "phi");
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
	glUniform1f(phiID, this->lastDraw);
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
	GLuint textID = glGetUniformLocation(shader->getId(), "textSampler1");
	glUniform1i(textID, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->nfrontID);
	textID = glGetUniformLocation(shader->getId(), "textSampler2");
	glUniform1i(textID, 1);
	this->front->draw(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->upID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->nupID);
	this->up->draw(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->rightID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->nrightID);
	this->right->draw(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->backID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->nbackID);
	this->back->draw(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->downID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->ndownID);
	this->down->draw(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->leftID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->nleftID);
	this->left2->draw(0);
	//this->left2->draw(0);
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