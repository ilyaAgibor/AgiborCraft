#pragma once
#include "glm/glm.hpp"
#include "OGL3D/Renderer.h"
#include "OGL3D/MVP.h"
#include "OGL3D/VertexBufferLayout.h"

enum BlockType {
	DIRT = 0,
	STONE = 1,
	SAND = 2,
	DIAMOND = 3,
	COAL = 4,
	WHITE = 5,
	RED = 6,
	GRASS = 7
};

#define FRONT  0x1
#define BACK   0x2
#define TOP    0x4
#define BOTTOM 0x8
#define RIGHT  0x10
#define LEFT   0x20

class Block {
public:
	Block** n_right = nullptr;
	Block** n_left = nullptr;
	Block** n_front = nullptr;
	Block** n_back = nullptr;
	Block** n_top = nullptr;
	Block** n_bottom = nullptr;
	Block(int x, int y, int z, BlockType type);
	glm::vec3 GetPosition() const;
	glm::vec3 GetLocalPosition() const;
	void SetLocalPosition(int x, int y, int z);
	bool GetVisable();
	void SetVisable(bool var);
	void SetMask(const int mask);
	int GetMask() const;
	inline BlockType GetType() const { return type; }
	inline void SetType(BlockType t) { type = t; }
private:
	BlockType type;
	bool visable = true;
	glm::vec3 position;
	glm::vec3 local_position;
	int mask;
};

