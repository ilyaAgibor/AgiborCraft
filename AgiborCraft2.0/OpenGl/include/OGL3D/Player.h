#pragma once
#include "Packet.h"
#include "OGL3D/Camera.h"
#include "World.h"
#include <cmath>



class Player {
private:
	int hit_range = 120;
	BlockType selected;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 max_speed = glm::vec3(25,25,25);
	float acceleration;
	float friction;
	Camera* cam;
	MVP* mvp;
	Block* FindClosestBlock(Chunk** c);
	
public:
	Player(MVP* mvp ,float speed, float friction);
	~Player();
	void Controls(float delta_time);
	Packet Break(World& world, Renderer& r);
	void Input();
	glm::vec3 GetPosition() const { return this->position; }
	glm::vec2 GetDirection() const { return this->cam->GetAngle();}
};
