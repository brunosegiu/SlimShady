#pragma once
#include "Mesh.h"
class MeshInstanced : public Mesh {
public:
	unsigned int instancesCount;
	GLuint instancesCoordID;
	vector<float> instancesCoords;
public:
	//Carga coordenadas para el mesh
	void loadCoords(string path);
	//lee coordenadas de xml
	MeshInstanced(string modelPath, string coordsPath);
	//lee coordenadas del mismo float[] que se le pasa
	MeshInstanced(string modelPath, vector<float> coords);
	//Pense en sobrecargar la operacion, pero mejor darle otro metodo
	void drawInstanced(GLuint shaderID);

};