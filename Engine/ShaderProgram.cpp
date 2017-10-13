#include "ShaderProgram.h"
#include <fstream>
#include <iostream>

ShaderProgram::ShaderProgram(){
    programId = NULL; //NULL = 0
	vertexId = NULL;
	fragmentId = NULL;
	geometryId = NULL;
}

GLuint ShaderProgram::getId(){
    return programId;
}

bool ShaderProgram::bind() {
	glUseProgram(programId);
	GLenum err = glGetError();
	if (err != GLEW_NO_ERROR) {
		printf("Error binding shader! %s\n", gluErrorString(err));
		printProgramLog();
		return false;
	}
	return true;
}

void ShaderProgram::unbind() {
	glUseProgram(NULL);			//Deja que se renderice todo con el pipeline estatico (depende de la version de opengl)
}

bool ShaderProgram::loadProgram() {
	GLint success = GL_TRUE;
	programId = glCreateProgram();
	if (vertexId) glAttachShader(programId, vertexId);
	if (fragmentId) glAttachShader(programId, fragmentId);
	if (geometryId) glAttachShader(programId, geometryId);

	glLinkProgram(programId);
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (success != GL_TRUE)
	{
		printf("Error linking program %d!\n", programId);
		printProgramLog();
		return false;
	}
	if (vertexId) { glDetachShader(programId, vertexId); glDeleteShader(vertexId); };
	if (fragmentId) { glDetachShader(programId, fragmentId); glDeleteShader(fragmentId); };
	if (geometryId) { glDetachShader(programId, geometryId); glDeleteShader(geometryId); };
	return true;
}

void ShaderProgram::loadShader(std::string path, GLenum type) { 
	std::string shaderString;
	std::ifstream sourceFile(path.c_str());
	if (sourceFile)
	{
		shaderString.assign((std::istreambuf_iterator< char >(sourceFile)), std::istreambuf_iterator< char >());
		GLuint shaderId = glCreateShader(type);
		const GLchar* shaderSource = shaderString.c_str();
		glShaderSource(shaderId, 1, (const GLchar**)&shaderSource, NULL);
		glCompileShader(shaderId);
		GLint shaderCompiled = GL_FALSE;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderCompiled);
		if (shaderCompiled != GL_TRUE)
		{
			printf("Unable to compile shader %d!\n\nSource:\n%s\n", shaderId, shaderSource);
			printShaderLog(shaderId);
			glDeleteShader(shaderId);
			shaderId = NULL;
		}
		else {
			switch (type) {
			case GL_VERTEX_SHADER: vertexId = shaderId;
			case GL_FRAGMENT_SHADER: fragmentId = shaderId;
			case GL_GEOMETRY_SHADER: geometryId = shaderId;
			}
		}
	} else{
		printf("Unable to open file %s\n", path.c_str());
	}

	/*
	Pensar bien como hacer esto. Cómo para cargar un shader sólo se precisa un string, podrian ponerse los sources
	del vertex shader y del fragment shader en el mismo archivo y cargar de alguna forma seleccionando el pedazo de código del vertex 
	y del fragment por separado.
	*/
}

void ShaderProgram::printProgramLog() {
	if (glIsProgram(programId))
	{
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);

		char* infoLog = new char[maxLength];

		glGetProgramInfoLog(programId, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			printf("%s\n", infoLog);
		}
		else printf("No log\n");

		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a program\n", programId);
	}
}

void ShaderProgram::printShaderLog(GLuint shader)
{
	if (glIsShader(shader))
	{
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		char* infoLog = new char[maxLength];

		glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			printf("%s\n", infoLog);
		}
		else printf("No log\n");

		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a shader\n", shader);
	}
}

ShaderProgram::~ShaderProgram(){
    glDeleteProgram(programId);
}

/*
Para usar el shader program se debe hacer algo del estilo:

ShaderProgram shader;
shader.loadShader("shaders/vertex_source.glvs", GL_VERTEX_SHADER)
shader.loadShader("shaders/fragment_source.glfs", GL_FRAGMENT_SHADER);
... (MOar ShaDErz)
if (!shader.loadProgram()) {
printf("Unable to load basic shader!\n");
return false;
}
shader.bind();
*/