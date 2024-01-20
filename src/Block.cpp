#include "Block.h"


Block::Block(int x, int y, int z, BlockType type) : type(type) {
	this->position = glm::vec3(x,y,z);
	this->local_position = glm::vec3(x,y,z);

}

glm::vec3 Block::GetPosition() const
{
	return this->position;
}

glm::vec3 Block::GetLocalPosition() const
{
	return this->local_position;
}

void Block::SetLocalPosition(int x, int y, int z)
{
	this->local_position = glm::vec3(x,y,z);
}

bool Block::GetVisable()
{
	return this->visable;
}

void Block::SetVisable(bool var)
{
	this->visable = var;
}

int Block::GetMask() const
{
	return this->mask;
}

void Block::SetMask(const int mask)
{
	this->mask = mask;
}

