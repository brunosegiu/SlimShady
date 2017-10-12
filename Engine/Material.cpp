#include "Material.h"

#include <FreeImage.h>

Material::Material() {

}

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, std::string text) {
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->isTextured = text != "";
	if (isTextured) {
		FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(text.c_str());
		FIBITMAP* bitmap = FreeImage_Load(fif, text.c_str());
		bitmap = FreeImage_ConvertTo24Bits(bitmap);

		glGenTextures(1, &this->textID);
		glBindTexture(GL_TEXTURE_2D, this->textID);
		int h = FreeImage_GetHeight(bitmap);
		int w = FreeImage_GetWidth(bitmap);
		void* pixels = FreeImage_GetBits(bitmap);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels);

		FreeImage_Unload(bitmap);
	}
}

Material::~Material() {
	if (isTextured) {
		glDeleteTextures(1, &this->textID);
	}
}