#pragma once
#include "Chunk.h"

#define RENDER_DISTANCE 6


class World {
public:
	World();
	~World();
	static void WorldToLocalChunk(int& x, int& y);
	void Draw(Renderer& r, MVP& mvp);
	Chunk* GetChunkFromWorldPosition(int x, int y);
	void DestroyBlock(int x, int y, int z);
	//void Logic(Player& p);
private:
	Chunk* chunks[RENDER_DISTANCE][RENDER_DISTANCE];
};