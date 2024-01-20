#include "OGL3D/TextureArray.h"

TextureArray::TextureArray(int size, int w, int h, int bpp) : size(size), width(w), height(h), bpp(bpp)
{
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->id);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, 16, 16, size);
}

TextureArray::~TextureArray()
{
	glDeleteTextures(1, &this->id);
}

void TextureArray::AddTexture(const std::string& path)
{
	stbi_set_flip_vertically_on_load(1);
	int w, h, temp3;
	unsigned char* buffer = stbi_load(path.c_str(), &w, &h, &temp3, 4);

	glBindTexture(GL_TEXTURE_2D_ARRAY, this->id);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, this->index, w, h, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	this->index += 1;

	if (buffer)
		stbi_image_free(buffer);
}



void TextureArray::Bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->id);
}

void TextureArray::UnBind()
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

}
