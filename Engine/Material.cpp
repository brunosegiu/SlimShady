#include "Material.h"

#include <exception>
#include <iostream>
#include<fstream>
#include<string>
#include<iterator>

#include <FreeImage.h>

Material::Material() {

}

GLuint Material::loadTexture(std::string path) {
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

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, std::string text, std::string normal) {
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->isTextured = text != "";
	this->isNormalMapped = isTextured && normal != "";
	if (isTextured)
		this->textID = loadTexture(text);
	if (isNormalMapped)
		this->normalID = loadTexture(normal);
}

std::vector<Material*> Material::loadMtl(string path) {
	string mtlExt = path + ".mtl";
	string line;
	ifstream mtlFile(mtlExt.c_str());
	vector<Material*> mats;
	if (mtlFile.is_open()) {

		unsigned int inserted = 0;
		string currentMtl = "";
		string prefixMTL = "newmtl";
		string prefixKa = "Ka";
		string prefixKd = "Kd";
		string prefixKs = "Ks";
		string prefixNs = "Ns";
		string prefixMapKd = "map_Kd";
		string prefixMapBump = "map_Bump";

		float aux1, aux2, aux3;
		float ambient1, ambient2, ambient3 = 0;
		float diffuse1, diffuse2, diffuse3 = 0;
		float specular1, specular2, specular3 = 0;
		string texturePath = "";
		string normalPath = "";

		while (!mtlFile.eof()) {
			getline(mtlFile, line);
			if (!strncmp(line.c_str(), prefixMTL.c_str(), strlen(prefixMTL.c_str()))) {
				inserted++;
				if (currentMtl != "") {
					Material* mat = new Material(glm::vec3(ambient1, ambient2, ambient3), glm::vec3(diffuse1, diffuse2, diffuse3), glm::vec3(specular1, specular2, specular3), texturePath, normalPath);
					texturePath = "";
					normalPath = "";
					mats.push_back(mat);
				}
				currentMtl = line.substr(7, line.length());
			}
			else if (!strncmp(line.c_str(), prefixKa.c_str(), strlen(prefixKa.c_str()))) {
				string data = line.substr(strlen(prefixKa.c_str()) - 1, strlen(line.c_str()));
				data[0] = data[1] = ' ';
				sscanf_s(data.c_str(), "%f %f %f", &aux1, &aux2, &aux3);
				ambient1 = aux1;
				ambient2 = aux2;
				ambient3 = aux3;
			}
			else if (!strncmp(line.c_str(), prefixKd.c_str(), strlen(prefixKd.c_str()))) {
				string data = line.substr(strlen(prefixKd.c_str()) - 1, strlen(line.c_str())).c_str();
				data[0] = data[1] = ' ';
				sscanf_s(data.c_str(), "%f %f %f", &aux1, &aux2, &aux3);
				diffuse1 = aux1;
				diffuse2 = aux2;
				diffuse3 = aux3;
			}
			else if (!strncmp(line.c_str(), prefixKs.c_str(), strlen(prefixKs.c_str()))) {
				string data = line.substr(strlen(prefixKs.c_str()) - 1, strlen(line.c_str())).c_str();
				sscanf_s(data.c_str(), "%f %f %f", &aux1, &aux2, &aux3);
				specular1 = aux1;
				specular2 = aux2;
				specular3 = aux3;
			}
			else if (!strncmp(line.c_str(), prefixMapKd.c_str(), strlen(prefixMapKd.c_str()))) {
				texturePath = line.substr(strlen(prefixMapKd.c_str()) + 1, strlen(line.c_str())).c_str();
			}
			else if (!strncmp(line.c_str(), prefixMapBump.c_str(), strlen(prefixMapBump.c_str()))) {
				normalPath = line.substr(strlen(prefixMapBump.c_str()) + 1, strlen(line.c_str())).c_str();
			}
		}

		if (currentMtl != "" && mats.size() < inserted) {
			mats.push_back(new Material(glm::vec3(ambient1, ambient2, ambient3), glm::vec3(diffuse1, diffuse2, diffuse3), glm::vec3(specular1, specular2, specular3), texturePath, normalPath));
			texturePath = "";
			normalPath = "";
		}

		mtlFile.close();
		return mats;
	}
	else {
		throw runtime_error("Unable to load mtl file: " + path + ".mtl");
	}
}

Material::~Material() {
	glDeleteTextures(1, &this->textID);
	glDeleteTextures(1, &this->normalID);
}