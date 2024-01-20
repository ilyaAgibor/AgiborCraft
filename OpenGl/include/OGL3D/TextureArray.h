#pragma once
#include <string>
#include "stb/stb_image.h"
#include "GLEW/glew.h"

class TextureArray {
public:
	TextureArray(int size, int w, int h, int bpp);
	~TextureArray();

	void AddTexture(const std::string& path);

	void Bind(unsigned int slot = 0);
	void UnBind();

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
	inline int GetSize() const { return size; }
	inline unsigned int GetGLId() const { return id; }

private:
	int index = 0;
	int size;
	int width, height, bpp;
	unsigned int id;
};