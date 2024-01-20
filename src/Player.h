#pragma once
#define NOMINMAX
#include "Packet.h"
#include "Camera.h"
#include "World.h"
#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtx/compatibility.hpp"


class Player {
private:
	int hit_range = 120;
	BlockType selected;
	glm::vec3 position;
	glm::vec3 velocity;
	float max_vel = 8.0f;
	glm::vec3 acceleration;
	float speed = 30.0f;
	float friction;
	float s_friction = 1.8f;
	Camera* cam;
	MVP* mvp;
	Block* FindClosestBlock(Chunk** c);
	float gravity = 9.8f;
	float height = 2.8f;
	bool godmode = false;
	bool is_grounded = false;
	bool is_jumping = false;
	float jump_height = 0;
	float max_jump_height = 2;
	float jumpforce = 1500.0f;
	bool CheckGrounded(World& world, Block** b);
	bool Collisions(World& world);
	float thickness = 0.2f;
	bool colliding_x = false;
	bool colliding_y = false;
	glm::vec2 off;
public:
	Player(MVP* mvp ,float speed, float friction);
	~Player();
	void Controls(float delta_time);
	void physics(float delta_time);
	void Logic(World& world, float delta_time);	
	void AddForce(glm::vec3 f);
	Packet Break(World& world, Renderer& r);
	void Input();
	glm::vec3 GetPosition() const { return this->position; }
	glm::vec2 GetDirection() const { return this->cam->GetAngle();}
};
