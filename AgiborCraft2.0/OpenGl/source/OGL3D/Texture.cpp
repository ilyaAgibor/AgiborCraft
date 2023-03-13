#include "OGL3D/Texture.h"

Texture::Texture(const std::string& path) : file_path(path), local_buffer(nullptr), height(0), width(0), BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	local_buffer = stbi_load(path.c_str(), &this->width, &this->height, &this->BPP, 4);

	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->local_buffer);
	glBindTexture(GL_TEXTURE_2D, 0);

}

Texture::~Texture()
{
	glDeleteTextures(1, &this->id);
	if (local_buffer)
		stbi_image_free(local_buffer);
}

void Texture::Bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, this->id);

}

void Texture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);

}
