#pragma once
#include <GL/glew.h>
#include "defines.h"

struct IndexBuffer
{
	IndexBuffer(void* data, uint32_t numIndices,uint8_t elementsize) {


		glGenBuffers(1, &bufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * elementsize, data, GL_STATIC_DRAW);


	}

	virtual ~IndexBuffer() {
		glDeleteBuffers(1, &bufferID);
	}
	void bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
	}
	void unBind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

private:
	GLuint bufferID;
	GLuint vao;

};
