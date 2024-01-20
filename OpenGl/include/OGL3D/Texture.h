#pragma once
#include "GLEW/glew.h"
#include "stb/stb_image.h"
#include <string>

class Texture {
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0);
	void UnBind();

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
	inline unsigned int GetGLId() const { return id; }
	inline unsigned char* GetBuffer() const { return local_buffer; }
private:
	unsigned int id;
	std::string file_path;
	unsigned char* local_buffer;
	int width, height, BPP;
};

