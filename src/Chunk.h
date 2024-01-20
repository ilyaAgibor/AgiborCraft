#pragma once
#include "Block.h"
#include "InstanceBuffer.h"
#include "PerlinNoise.h"
#include <cstring>
#include <vector>

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 16
#define CHUNK_SIZE_Z 256

#define CHUNK_VOLUME (CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z)

class Chunk {
public:
	Chunk** n_front = nullptr;
	Chunk** n_back = nullptr;
	Chunk** n_left = nullptr;
	Chunk** n_right = nullptr;
	int left_x;
	int right_x;
	int front_y;
	int back_y;
	int neighbors;
	Block* chunk_blocks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
	Chunk(int x , int y);
	Chunk(int x, int y, int* height_map);
	~Chunk();
	bool DestroyBlock(int x, int y, int z);
	bool BuildBlock(int x, int y, int z, BlockType t);
	Block* CheckBlock(int x, int y, int z);
	signed char GetBlockType(int x, int y, int z) {
		if (chunk_blocks[x][y][z])
			return chunk_blocks[x][y][z]->GetType();
		return -1;
	}
	void Draw(Renderer& r, MVP& mvp);
	void UpdateNeighbors();
	void UpdateMasks();
	void UpdateInstances();
	void Clear();
	glm::vec2 GetOffset();

private:
	std::vector<glm::mat4>* cube_instances;
	std::vector<float>* texture_indeces;
	InstanceBuffer* pinb;
	int x_offset;
	int y_offset;
};