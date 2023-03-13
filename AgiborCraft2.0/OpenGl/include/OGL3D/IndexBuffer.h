#pragma once
#include <GLEW/glew.h>

class IndexBuffer {
public:
	IndexBuffer(void* buffer, unsigned int count);
	~IndexBuffer();
	void bind();
	void unbind();
	unsigned int GetCount();
private:
	unsigned int gl_id;
	unsigned int count;
	void* buffer;
};