#include "OGL3D/World.h"


World::World()
{
	memset(chunks, 0, sizeof(chunks));
	for (int x = -RENDER_DISTANCE/2; x < RENDER_DISTANCE/2; x++) {
		for (int y = -RENDER_DISTANCE/2; y < RENDER_DISTANCE/2; y++) {
			chunks[x+RENDER_DISTANCE/2][y+RENDER_DISTANCE/2] = new Chunk(x, y);
		}
	}
	int n = 0;
	for(int x = 0; x < RENDER_DISTANCE; x++)
		for (int y = 0; y < RENDER_DISTANCE; y++) {
			Chunk& c = *chunks[x][y];
			if (x > 0) {
				c.n_left = &chunks[x - 1][y];
				n++;
			}
			if (x < RENDER_DISTANCE - 1) {
				c.n_right = &chunks[x + 1][y];
				n++;
			}
			if (y > 0) {
				c.n_back = &chunks[x][y - 1];
				n++;
			}
			if (y < RENDER_DISTANCE - 1) {
				c.n_front = &chunks[x][y + 1];
				n++;
			}
			c.neighbors = n;
		}
	for (int x = 0; x < RENDER_DISTANCE; x++)
		for (int y = 0; y < RENDER_DISTANCE; y++) {
			Chunk& c = *chunks[x][y];
			c.UpdateNeighbors();
			c.UpdateMasks();
			c.UpdateInstances();
		}

	std::cout << "Finished creating world!\n" << std::endl;
}

World::~World()
{
	for (int x = 0; x < RENDER_DISTANCE; x++) {
		for (int y = 0; y < RENDER_DISTANCE; y++) {
				if (chunks[x][y] != nullptr)
					delete chunks[x][y];
		}
	}
}

void World::WorldToLocalChunk(int& x, int& y)
{
	x = (x - ((x / CHUNK_SIZE_X) * CHUNK_SIZE_X));
	y = (y - ((y / CHUNK_SIZE_Y) * CHUNK_SIZE_Y));
	if (x < 0)
		x += CHUNK_SIZE_X;
	if (y < 0)
		y += CHUNK_SIZE_Y;
}

void World::Draw(Renderer& r, MVP& mvp)
{
	for (int x = 0; x < RENDER_DISTANCE; x++) {
		for (int y = 0; y < RENDER_DISTANCE; y++) {
				if (chunks[x][y] == nullptr) continue;
					chunks[x][y]->Draw(r, mvp);
		}
	}
}

Chunk* World::GetChunkFromWorldPosition(int x, int y)
{
	if (x < 0) x -= 16;
	if (y < 0) y -= 16;
	x = (x/CHUNK_SIZE_X)+(RENDER_DISTANCE/2);
	y = (y/CHUNK_SIZE_Y)+(RENDER_DISTANCE/2);
	
	if ((x >= 0 && y >= 0) && (x < RENDER_DISTANCE && y < RENDER_DISTANCE))
		return chunks[x][y];
	else
		return nullptr;
}

void World::DestroyBlock(int x, int y, int z)
{
	Chunk* c = this->GetChunkFromWorldPosition(x,y);
	this->WorldToLocalChunk(x,y);
	c->DestroyBlock(x,y,z);
}

//void World::Logic(Player& p)
//{
//
//}
