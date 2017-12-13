#include "Grass.h"
#include <FreeImage.h>
#include <ctime>

Grass::Grass(std::vector<float> positions, std::string texPath, int sizeX, int sizeY, int density) : Model() {
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	this->density = density;
	this->positions = positions;
	this->texID = loadTexture(texPath);
	this->shader = new ShaderProgram("assets/shaders/grass.vert", "assets/shaders/grass.geom", "assets/shaders/grass.frag");
	this->fGrassPatchOffsetMin = 1.5f;
	this->fGrassPatchOffsetMax = 2.5f;
	this->fGrassPatchHeight= 5.0f;
	this->time = 0.0f;

	if ((density > 1) && (positions.size() > 5)) {
		std::vector<float> auxPositions;
		while (density > 1) {
			//tomo promedio entre 2 vertices contiguos y lo agrego al montón
			for (int i = 0; i < positions.size() - 5; i+=6) {
				glm::vec3 position = glm::vec3(positions.at(i) + positions.at(i + 4), positions.at(i + 1) + positions.at(i + 5), positions.at(i + 2) + positions.at(i + 6));
				position = position*0.5f;
				auxPositions.push_back((float)position.x);
				auxPositions.push_back((float)position.y);
				auxPositions.push_back((float)position.z);
			}
			density--;
		}
		
		positions.insert(positions.end(),auxPositions.begin(),auxPositions.end());
	}

	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*positions.size(), &positions[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	this->vertexCount = positions.size() / 3;

	//unbinds
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

GLuint Grass::loadTexture(std::string path) {
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

void Grass::update(float time, glm::vec3 vEyePosition){
	this->time += time;
	this->vEyePosition = vEyePosition;
}
void Grass::draw(GLuint shaderID) {
	
	this->shader->bind();
	
	float fAlphaTest = 0.25f;
	float fAlphaMultiplier = 1.5f;

	GLuint projMatrixID = glGetUniformLocation(shader->getId(), "projMatrix");
	GLuint modelMatrixID = glGetUniformLocation(shader->getId(), "modelMatrix");
	GLuint viewMatrixID = glGetUniformLocation(shader->getId(), "viewMatrix");
	GLuint fTimePassedID = glGetUniformLocation(shader->getId(), "fTimePassed");
	GLuint gSamplerID = glGetUniformLocation(shader->getId(), "gSampler");
	GLuint vEyePositionID = glGetUniformLocation(shader->getId(), "vEyePosition");
	GLuint fAlphaTestID = glGetUniformLocation(shader->getId(), "fAlphaTest");
	GLuint fAlphaMultiplierID = glGetUniformLocation(shader->getId(), "fAlphaMultiplier");
	GLuint lightDirID = glGetUniformLocation(shaderID, "lightdir");
	GLuint lightColorID = glGetUniformLocation(shaderID, "lightcolor");
	GLuint moonDirID = glGetUniformLocation(shaderID, "moondir");
	GLuint moonColorID = glGetUniformLocation(shaderID, "mooncolor");

	glUniformMatrix4fv(projMatrixID, 1, GL_FALSE, &projMatrix[0][0]);
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniform1f(fTimePassedID, time);
	glUniform3fv(vEyePositionID, 1, &vEyePosition[0]);
	glUniform1f(fAlphaTestID, fAlphaTest);
	glUniform1f(fAlphaMultiplierID, fAlphaMultiplier);
	glUniform3fv(lightDirID, 1, &l1[0]);
	glUniform3fv(lightColorID, 1, &(i1)[0]);
	glUniform3fv(moonDirID, 1, &l2[0]);
	glUniform3fv(moonColorID, 1, &(i2)[0]);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texID);
	glUniform1i(gSamplerID, 0);

	/*
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	*/
	glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);
	glDisable(GL_CULL_FACE);
	glDrawArrays(GL_POINTS, 0, vertexCount);
	glEnable(GL_CULL_FACE);

	glDisableVertexAttribArray(0);
}

Grass::~Grass()
{
	delete shader;
}
