#include "VertexBuffer.h"
#include <iostream>

VertexBuffer::VertexBuffer(void* buffer, unsigned int size) {
	unsigned int tmp;
	glGenBuffers(1, &tmp);
	this->gl_id = tmp;
	this->bind();
	glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
	this->size = size;
	this->buffer = buffer;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, this->gl_id);	
}

void VertexBuffer::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &this->gl_id);
}