#pragma once
#include <string>
#include <GL/glew.h>
#include "defines.h"

struct meshShader
{
	meshShader(const char* vertexShaderFilename, const  char* fragmentShaderFilename);
	virtual ~meshShader();
	void bind();
	void unbind();
	GLuint getShaderId() {
		return shaderID;
	}

private:
	GLuint compile(std::string shaderSource,GLenum type);
	std::string parse(const char* filename);
	GLuint createShader(const char* vertexShaderFilename, const char* fragmentShaderFilename);

	GLuint shaderID;


};

