#pragma once
#include "Chunk.h"
#include <random>
#include <cstring>
#include "PerlinNoise.h"
#include <unordered_map>
#include <functional>



#define RENDER_DISTANCE 8
#define NOISE_SIZE 16 * 16 * RENDER_DISTANCE

typedef struct _position {
	int x;
	int y;
	int z;
} Position;


class World {
public:
	World(unsigned int def_seed = 0);
	~World();
	static void WorldToLocalChunk(int& x, int& y);
	void Draw(Renderer& r, MVP& mvp);
	Chunk* GetChunkFromWorldPosition(int x, int y);
	bool DestroyBlock(int x, int y, int z);
	bool PlaceBlock(int x, int y, int z, int type);
	Block* CheckBlock(int x, int y, int z);
	Chunk* Data() { return chunks[0][0]; }
	unsigned int GetWorldSeed() const { return world_seed; }
private:
	unsigned int world_seed;
	void FillWorldData();
	Chunk* chunks[RENDER_DISTANCE][RENDER_DISTANCE];
	char* world_raw_data;
};