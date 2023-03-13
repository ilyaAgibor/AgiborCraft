#include "OGL3D/IndexBuffer.h"
#include <iostream>

IndexBuffer::IndexBuffer(void* buffer, unsigned int count) {
	unsigned int tmp;
	glGenBuffers(1, &tmp);
	this->gl_id = tmp;
	this->count = count;
	this->buffer = buffer;
	this->bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), buffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &this->gl_id);
}

unsigned int IndexBuffer::GetCount() {
	return this->count;
}

void IndexBuffer::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->gl_id);
}

void IndexBuffer::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
