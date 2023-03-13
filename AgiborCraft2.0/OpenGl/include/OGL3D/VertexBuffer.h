#pragma once
#include <GLEW/glew.h>

class VertexBuffer {
public:
	VertexBuffer(void* buffer,unsigned int size);
	~VertexBuffer();
	void bind();
	void unbind();
private:
	unsigned int gl_id;
	int size;
	void* buffer;

};