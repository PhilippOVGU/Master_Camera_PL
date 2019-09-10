#pragma once
#include <GL/glew.h>
#include "defines.h"


struct VertexBuffer
{
	VertexBuffer(void* data, uint32_t numVertices) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		GLuint bufferID = rand() % 100;
		std::cout << "Buffer id: " << bufferID << std::endl;
		glGenBuffers(1, &bufferID); // creates the id for the buffer and uses that id
		glBindBuffer(GL_ARRAY_BUFFER, bufferID); // creates the id for the buffer and uses that id
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), data, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, x));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, u));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, r));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, nx));
		
	}

	virtual ~VertexBuffer() {
		glDeleteBuffers(1, &bufferID);
	}
	void bind() {
		glBindVertexArray(vao);
	}
	void unBind() {
		glBindVertexArray(0);
	}
	
	
	void newData(void* data, uint32_t numVertices) { 

		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), data, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, x));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, u));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, r));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, nx));


	}

private:
	GLuint bufferID;
	GLuint vao;

};
#pragma once
