#pragma once
#include "GLEW/glew.h"
#include <vector>

class TextureIndexBuffer {
public:
	TextureIndexBuffer();
	~TextureIndexBuffer();
	void AddIndex(float index);
	void bind();
	void unbind();
	void SetIndecesBuffer(std::vector<float>* instances);
	int NumOfIndeces();
private:
	std::vector<float>* indeces;
	unsigned int gl_id;
};