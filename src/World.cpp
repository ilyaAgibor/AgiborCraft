#include "World.h"



World::World(unsigned int def_seed)
{
	int* map = new int[CHUNK_SIZE_X*RENDER_DISTANCE*NOISE_SIZE+CHUNK_SIZE_Y*RENDER_DISTANCE];
	if (def_seed == 0) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<unsigned short> dist(0, std::numeric_limits<unsigned short>::max());
		world_seed = dist(gen);
	}
	else
	{
		world_seed = def_seed;
	}
	PerlinNoise noise(world_seed);

	int borderValue = 70; // replace with desired value
	int maxHeight = 120;

	// Generate the map
	for (int i = 0; i < CHUNK_SIZE_X * RENDER_DISTANCE; i++) {
		for (int j = 0; j < CHUNK_SIZE_Y * RENDER_DISTANCE; j++) {
			map[i * NOISE_SIZE + j] = noise.noise_int(i / (double)(CHUNK_SIZE_X * RENDER_DISTANCE), j / (double)(CHUNK_SIZE_Y * RENDER_DISTANCE), 200.0f, 75);
		}
	}
	//shaked dan zilberman gave me the idea to do this at the date 4/21/2023, and wrote the following code (12:23):
	// multiply each height in `map` according to its radial distance of degree p (deg 2 if pythagorian) (x^p + y^p) and the function e^{-4 a^{-2} r_deg_p}.
	int x0 = 64, y0 = 64;
	double a = 64;
	int p = 6;
	double am2 = pow(a, -p);
	for (int x = 0; x < CHUNK_SIZE_X * RENDER_DISTANCE; x++) {
		for (int y = 0; y < CHUNK_SIZE_Y * RENDER_DISTANCE; y++) {
			int index = x * NOISE_SIZE + y;
			double r2 = pow(x - x0, p) + pow(y - y0, p);
			map[index] = 40 + 0.3 * map[index] * exp(-4 * am2 * r2);
		}
	}

	memset(chunks, 0, sizeof(chunks));
	for (int x = 0; x < RENDER_DISTANCE; x++) {
		for (int y = 0; y < RENDER_DISTANCE; y++) {
			chunks[x][y] = new Chunk(x, y, map);
		}
	}
	std::cout << "finished creating chunks" << std::endl;
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
	int count = 0;
	for (int x = 0; x < RENDER_DISTANCE; x++)
		for (int y = 0; y < RENDER_DISTANCE; y++) {
			Chunk& c = *chunks[x][y];
			c.UpdateNeighbors();
			c.UpdateMasks();
			c.UpdateInstances();
			count++;
		}
	/*for (int i = 0; i < NOISE_SIZE; i++) {
		for (int j = 0; j < NOISE_SIZE; j++) {
			map[i*(NOISE_SIZE-1)+j] = noise.octaves(i / (float)NOISE_SIZE, j / (float)NOISE_SIZE, 8, 0.5);
		}
	}
	printf("%f %f %f %f\n", map[0], map[1], map[2], map[3]);*/
	delete[] map;
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
	x = (x / CHUNK_SIZE_X);
		y = (y / CHUNK_SIZE_Y);
	
	if ((x >= 0 && y >= 0) && (x < RENDER_DISTANCE && y < RENDER_DISTANCE))
		return chunks[x][y];
	else
		return nullptr;
}



bool World::DestroyBlock(int x, int y, int z)
{
	Chunk* c = this->GetChunkFromWorldPosition(x,y);
	this->WorldToLocalChunk(x,y);
	bool ret = c->DestroyBlock(x,y,z);
	c->UpdateInstances();
	if (c->n_right && *c->n_right) {
		(*c->n_right)->UpdateInstances();
	}
	if (c->n_left && *c->n_left) {
		(*c->n_left)->UpdateInstances();

	}
	if (c->n_front && *c->n_front) {
		(*c->n_front)->UpdateInstances();

	}
	if (c->n_back && *c->n_back) {
		(*c->n_back)->UpdateInstances();
	}
	return ret;
}

bool World::PlaceBlock(int x, int y, int z, int type)
{
	Chunk* c = this->GetChunkFromWorldPosition(x, y);
	this->WorldToLocalChunk(x, y);
	bool ret = c->BuildBlock(x, y, z, (BlockType)type);
	c->UpdateNeighbors();
	c->UpdateMasks();
	c->UpdateInstances();
	return ret;
}

Block* World::CheckBlock(int x, int y, int z)
{
	Chunk* c = this->GetChunkFromWorldPosition(x, y);
	if (c) {
		this->WorldToLocalChunk(x, y);
		return c->CheckBlock(x, y, z);
	}
	return 0;
}

void World::FillWorldData()
{
	for (int i = 0; i < RENDER_DISTANCE; i++) {
		for (int j = 0; j < RENDER_DISTANCE; j++) {
			Chunk* chunk = chunks[i][j];
			int chunk_offset = i * RENDER_DISTANCE * CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z + j * CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z;
			for (int k = 0; k < CHUNK_SIZE_X; k++) {
				for (int l = 0; l < CHUNK_SIZE_Y; l++) {
					for (int m = 0; m < CHUNK_SIZE_Z; m++) {
						char block = chunk->GetBlockType(k, l, m);
						int block_offset = k * CHUNK_SIZE_Y * CHUNK_SIZE_Z + l * CHUNK_SIZE_Z + m;
						int index = chunk_offset + block_offset;
						world_raw_data[index] = block;
					}
				}
			}
		}
	}
}

