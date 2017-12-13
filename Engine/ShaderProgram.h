#pragma once

#include <GL/glew.h>
#include <stdio.h>
#include <string>
/*
Guarda varios etapas de shader (vertex, fragment) en un solo objeto linkeable
El programa que est'e bindeado determina que shaders se usan al llamar a draw()
*/

class ShaderProgram{
public:
    ShaderProgram();
	ShaderProgram(std::string vertPath, std::string fragPath);
	ShaderProgram(std::string vertPath, std::string geomPath, std::string fragPath);
    bool bind();
    void unbind();
    GLuint getId();
	bool loadProgram();
	void loadShader(std::string path, GLenum type); //devolver bool?
	void printProgramLog();
	void printShaderLog(GLuint id);
    virtual ~ShaderProgram();

protected:
    GLuint programId, vertexId, fragmentId, geometryId;
};
