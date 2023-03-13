#include "OGL3D/Chunk.h"

Chunk::Chunk(int x, int y)
{

	this->x_offset = x;
	this->y_offset = y;

	this->left_x = x*CHUNK_SIZE_X;
	this->back_y = y*CHUNK_SIZE_Y;
	this->right_x = x*CHUNK_SIZE_X + 15;
	this->front_y = y*CHUNK_SIZE_Y + 15;
	//clock_t t1, t2;
	//t1 = clock();
	memset(chunk_blocks, 0, sizeof(chunk_blocks));
	this->cube_instances = new std::vector<glm::mat4>();
	this->texture_indeces = new std::vector<float>();
	for (int x = 0; x < CHUNK_SIZE_X; x++) {
		for (int y = 0; y < CHUNK_SIZE_Y; y++) {
			for (int z = 0; z < 126; z++) {
				BlockType a;
				if (z < 100)
					a = STONE;
				else
					a = DIRT;
				chunk_blocks[x][y][z] = new Block(x + (x_offset * CHUNK_SIZE_X), y + (y_offset * CHUNK_SIZE_Y), z, a);
				chunk_blocks[x][y][z]->SetLocalPosition(x,y,z);
			}
		}
	}
	//t2 = clock();
	//float diff = ((float)t2 - (float)t1) / CLOCKS_PER_SEC;
	//std::cout << "chunk created in: " << diff << " seconds" << std::endl;
}

void Chunk::UpdateNeighbors()
{
	for (int x = 0; x < CHUNK_SIZE_X; x++) {
		for (int y = 0; y < CHUNK_SIZE_Y; y++) {
			for (int z = 0; z < CHUNK_SIZE_Z; z++) {
				if (chunk_blocks[x][y][z] == 0) continue;
				Block& block = *chunk_blocks[x][y][z];
				if (x > 0) {
					block.n_left = &chunk_blocks[x - 1][y][z];
				}
				else {
					if (this->n_left) {
						block.n_left = &(*this->n_left)->chunk_blocks[15][y][z];
					}
				}
				if (x < CHUNK_SIZE_X - 1) {
					block.n_right = &chunk_blocks[x + 1][y][z];
				}
				else
				{
					if (this->n_right) {
						block.n_right = &(*this->n_right)->chunk_blocks[0][y][z];
					}
				}
				if (y > 0) {
					block.n_back = &chunk_blocks[x][y - 1][z];

				}
				else
				{
					if (this->n_back) {
						block.n_back = &(*this->n_back)->chunk_blocks[x][15][z];
					}
				}
				if (y < CHUNK_SIZE_Y - 1) {
					block.n_front = &chunk_blocks[x][y + 1][z];
				}
				else
				{
					if (this->n_front) {
						block.n_front = &(*this->n_front)->chunk_blocks[x][0][z];
					}
				}

				if (z > 0)
					block.n_bottom = &chunk_blocks[x][y][z - 1];
				if (z < CHUNK_SIZE_Z - 1)
					block.n_top = &chunk_blocks[x][y][z + 1];
			}
		}
	}
}


Chunk::~Chunk()
{
	for (int x = 0; x < CHUNK_SIZE_X; x++) {
		for (int y = 0; y < CHUNK_SIZE_Y; y++) {
			for (int z = 0; z < CHUNK_SIZE_Z; z++) {
				if (chunk_blocks[x][y][z] != nullptr)
					delete chunk_blocks[x][y][z];
			}
		}
	}
	delete this->pinb;
	delete this->cube_instances;
	delete this->texture_indeces;
}

void Chunk::DestroyBlock(int x, int y, int z)
{
	if (x > CHUNK_SIZE_X || y > CHUNK_SIZE_Y || z > CHUNK_SIZE_Z) return;
	if (x < 0 || y < 0 || z < 0) return;
	if (chunk_blocks[x][y][z] == nullptr) return;
	
	Block& b = *chunk_blocks[x][y][z];

	if (b.n_left && *b.n_left != 0)
		(*b.n_left)->SetMask((*b.n_left)->GetMask() & RIGHT);
	if (b.n_right && *b.n_right)
		(*b.n_right)->SetMask((*b.n_right)->GetMask() & LEFT);

	if (b.n_top && *b.n_top)
		(*b.n_top)->SetMask((*b.n_top)->GetMask() & BOTTOM);
	if (b.n_bottom && *b.n_bottom)
		(*b.n_bottom)->SetMask((*b.n_bottom)->GetMask() & TOP);

	if (b.n_front && *b.n_front)
		(*b.n_front)->SetMask((*b.n_front)->GetMask() & BACK);
	if (b.n_back && *b.n_back)
		(*b.n_back)->SetMask((*b.n_back)->GetMask() & FRONT);

	delete chunk_blocks[x][y][z];
	chunk_blocks[x][y][z] = 0;
}

void Chunk::BuildBlock(int x, int y, int z, BlockType t)
{
	if (x > CHUNK_SIZE_X || y > CHUNK_SIZE_Y || z > CHUNK_SIZE_Z) return;
	if (x < 0 || y < 0 || z < 0) return;
	if (chunk_blocks[x][y][z] != nullptr) return;
	chunk_blocks[x][y][z] = new Block((x_offset*CHUNK_SIZE_X) + x,(y_offset*CHUNK_SIZE_Y)+ y, z, t);
	chunk_blocks[x][y][z]->SetLocalPosition(x,y,z);

}

void Chunk::Draw(Renderer& r, MVP& mvp)
{
	r.SetIdecesVector(this->texture_indeces);
	r.SetInstancesVector(this->cube_instances);
	mvp.PushMatrix();
	r.InstanceSetup();
	r.DrawInstances();
	mvp.PopMatrix();
}

void Chunk::UpdateMasks()
{
	for (int x = 0; x < CHUNK_SIZE_X; x++) {
		for (int y = 0; y < CHUNK_SIZE_Y; y++) {
			for (int z = 0; z < CHUNK_SIZE_Z; z++) {
				if (chunk_blocks[x][y][z] == 0) continue;
				Block& block = *chunk_blocks[x][y][z];
				int mask = 0;
				if (block.n_right != nullptr && *block.n_right != 0)
					mask |= RIGHT;
				if (block.n_left != nullptr && *block.n_left != 0)
					mask |= LEFT;
				if (block.n_front != nullptr && *block.n_front != 0)
					mask |= BACK;
				if (block.n_back != nullptr && *block.n_back != 0)
					mask |= FRONT;
				if (block.n_top != nullptr && *block.n_top != 0)
					mask |= TOP;
				if (block.n_bottom != nullptr && *block.n_bottom != 0)
					mask |= BOTTOM;
				block.SetMask(mask);
			}
		}
	}
}

void Chunk::UpdateInstances()
{
	this->cube_instances->clear();
	this->texture_indeces->clear();
	for (int x = 0; x < CHUNK_SIZE_X; x++) {
		for (int y = 0; y < CHUNK_SIZE_Y; y++) {
			for (int z = 0; z < CHUNK_SIZE_Z; z++) {
				if (chunk_blocks[x][y][z] == 0) continue;
				Block& block = *chunk_blocks[x][y][z];
				if (block.GetMask() != 0x3f) {
					glm::vec3 pos = block.GetPosition();
					glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(pos.x, pos.y, pos.z));
					this->cube_instances->push_back(model);
					this->texture_indeces->push_back(block.GetType());
				}
			}
		}
	}
}

void Chunk::Clear()
{
	memset(chunk_blocks, 0, sizeof(chunk_blocks));
	this->cube_instances->clear();
}

glm::vec2 Chunk::GetOffset()
{
	return glm::vec2(x_offset, y_offset);
}