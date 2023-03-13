#include "OGL3D/Player.h"



Player::Player(MVP* mvp, float ac, float friction) : mvp(mvp), acceleration(ac), friction(friction)
{
	this->selected = SAND;
	this->position = glm::vec3(0,0,128);
	this->velocity = glm::vec3(0,0,0);
	this->cam = new Camera(mvp, 0.1f);
}

Player::~Player() {
	delete this->cam;
}

void Player::Input() {
	if (GetKeyState('1') < 0) {
		this->selected = DIRT;
	}
	if (GetKeyState('2') < 0) {
		this->selected = STONE;
	}
	if (GetKeyState('3') < 0) {
		this->selected = SAND;
	}
	if (GetKeyState('4') < 0) {
		this->selected = DIAMOND;
	}
	if (GetKeyState('5') < 0) {
		this->selected = COAL;
	}
	if (GetKeyState('6') < 0) {
		this->selected = WHITE;
	}
	if (GetKeyState('7') < 0) {
		this->selected = RED;
	}
}

void Player::Controls(float delta_time)
{
	this->cam->MoveCamera(delta_time);
	constexpr float half = glm::radians(45.0f);
	if (GetKeyState('W') < 0) {
		if (this->velocity.y < this->max_speed.y)
			this->velocity.y += this->acceleration;
	}
	else if (GetKeyState('S') < 0) { 
		if (this->velocity.y > -this->max_speed.y)
			this->velocity.y -= this->acceleration;
	}
	else
	{
		this->velocity.y *= this->friction;
		if (this->velocity.y <= 0.01f) this->velocity.y = 0;
	}
	if (GetKeyState('A') < 0) {
		if (this->velocity.x > -this->max_speed.x)
			this->velocity.x -= this->acceleration;
	}
	else if (GetKeyState('D') < 0) {
		if (this->velocity.x < this->max_speed.x)
			this->velocity.x += this->acceleration;
	}
	else
	{
		this->velocity.x *= this->friction;
		if (this->velocity.x <= 0.01f) this->velocity.x = 0;
	}
	if (GetKeyState(VK_SPACE) < 0) {
		if (this->velocity.z < this->max_speed.z)
			this->velocity.z += this->acceleration;
	}
	else if (GetKeyState(VK_CONTROL) < 0) {
		if (this->velocity.z > -this->max_speed.z)
			this->velocity.z -= this->acceleration;
	}
	else
	{
		this->velocity.z *= this->friction;
		if (this->velocity.z <= 0.01f) this->velocity.z = 0;
	}

	this->velocity *= delta_time;
	this->position.x += this->velocity.x * std::sin(-glm::radians(this->cam->GetAngle().y - 90));
	this->position.y += this->velocity.x * std::cos(-glm::radians(this->cam->GetAngle().y - 90));
	this->position.x += this->velocity.y * std::sin(-glm::radians(this->cam->GetAngle().y));
	this->position.y += this->velocity.y * std::cos(-glm::radians(this->cam->GetAngle().y));
	this->position.z += this->velocity.z;

	this->mvp->Translate(glm::vec3(-this->position.x, -this->position.y, -this->position.z));
	this->Input();
}

glm::vec3 GetRayDirection(const Camera& camera) {
	double pitch = glm::radians(camera.GetAngle().x);
	double yaw = glm::radians(camera.GetAngle().y);

	double x = std::sin(pitch) * std::cos(yaw);
	double y = std::sin(pitch) * std::sin(yaw);
	double z = std::cos(pitch);
	return glm::vec3{ -y, x, -z };
}

Chunk* ChangeChunk(Chunk* c,float x, float y) {
	if (x > c->right_x) {
		if (c->n_right) {
			return (*c->n_right);
		}
		else if (!c->n_right) return nullptr;
	}
	if (x < c->left_x) {
		if (c->n_left) {
			return (*c->n_left);
		}
		else if (!c->n_left) return nullptr;
	}
	if (y > c->front_y) {
		if (c->n_front) {
			return (*c->n_front);
		}
		else if (!c->n_front) return nullptr;
	}
	if (y < c->back_y) {
		if (c->n_back) {
			return (*c->n_back);
		}
		else if (!c->n_back) return nullptr;
	}

	return c;
}

Block* Player::FindClosestBlock(Chunk** c) {
	
	// Initialize the ray's origin and direction
	glm::vec3 ray_direction = GetRayDirection(*this->cam);
	glm::vec3 ray_origin = this->position;

	// Initialize the closest block and distance to a large value
	Block* closest_block = nullptr;

	// Step through the 3D space along the ray's direction
	int x, y, z;
	for (float t = 1; t < hit_range; t += 0.3) {
		glm::vec3 ray_end = ray_origin + (ray_direction * t);
		// Convert the ray's end position to block coordinates
		x = (int)std::round(ray_end.x);
		y = (int)std::round(ray_end.y);
		z = (int)std::round(ray_end.z);
		*c = ChangeChunk(*c, x, y);
		if (*c == nullptr)
			return nullptr;
		World::WorldToLocalChunk(x, y);

		if ((x >= 0 && y >= 0 && z >= 0) && (x < CHUNK_SIZE_X && y < CHUNK_SIZE_Y && z < CHUNK_SIZE_Z)) {
			Block* block = (*c)->chunk_blocks[x][y][z];
			if (block != nullptr) {
				closest_block = block;
				break;
			}
		}
	}
	return closest_block;
}

glm::vec3 IntersectCube(const glm::vec3& ray_origin, const glm::vec3& ray_direction, const glm::vec3& position, float size) {
	glm::vec3 cube_max = position + glm::vec3(size);
	glm::vec3 cube_min = position - glm::vec3(size);
	float tMin = (cube_min.x - ray_origin.x) / ray_direction.x;
	float tMax = (cube_max.x - ray_origin.x) / ray_direction.x;

	if (tMin > tMax) {
		std::swap(tMin, tMax);
	}

	float tYMin = (cube_min.y - ray_origin.y) / ray_direction.y;
	float tYMax = (cube_max.y - ray_origin.y) / ray_direction.y;

	if (tYMin > tYMax) {
		std::swap(tYMin, tYMax);
	}

	if ((tMin > tYMax) || (tYMin > tMax)) {
		return glm::vec3(0, 0, -1);
	}
	if (tYMin > tMin) {
		tMin = tYMin;
	}

	if (tYMax < tMax) {
		tMax = tYMax;
	}

	float tZMin = (cube_min.z - ray_origin.z) / ray_direction.z;
	float tZMax = (cube_max.z - ray_origin.z) / ray_direction.z;

	if (tZMin > tZMax) {
		std::swap(tZMin, tZMax);
	}

	if ((tMin > tZMax) || (tZMin > tMax)) {
		return glm::vec3(0, 0, -1);
	}

	if (tZMin > tMin) {
		tMin = tZMin;
	}

	if (tZMax < tMax) {
		tMax = tZMax;
	}

	if (tMin == std::numeric_limits<float>::infinity() || tMin == -std::numeric_limits<float>::infinity()) {
		return glm::vec3(0, 0, -1);
	}

	if (ray_origin.x + tMin * ray_direction.x == cube_min.x) {
		return glm::vec3(-1, 0, 0);
	}
	else if (ray_origin.x + tMin * ray_direction.x == cube_max.x) {
		return glm::vec3(1, 0, 0);
	}
	else if (ray_origin.y + tMin * ray_direction.y == cube_min.y) {
		return glm::vec3(0, -1, 0);
	}
	else if (ray_origin.y + tMin * ray_direction.y == cube_max.y) {
		return glm::vec3(0, 1, 0);
	}
	else if (ray_origin.z + tMin * ray_direction.z == cube_min.z) {
		return glm::vec3(0, 0, -1);
	}
	else if (ray_origin.z + tMin * ray_direction.z == cube_max.z) {
		return glm::vec3(0, 0, 1);
	}

	if (ray_origin.x + tMax * ray_direction.x == cube_min.x) {
		return glm::vec3(-1, 0, 0);
	}
	else if (ray_origin.x + tMax * ray_direction.x == cube_max.x) {
		return glm::vec3(1, 0, 0);
	}
	else if (ray_origin.y + tMax * ray_direction.y == cube_min.y) {
		return glm::vec3(0, -1, 0);
	}
	else if (ray_origin.y + tMax * ray_direction.y == cube_max.y) {
		return glm::vec3(0, 1, 0);
	}
	else if (ray_origin.z + tMax * ray_direction.z == cube_min.z) {
		return glm::vec3(0, 0, -1);
	}
	else if(ray_origin.z + tMax * ray_direction.z == cube_max.z) {
		return glm::vec3(0, 0, 1);
	}
	return glm::vec3(0,0,0);
}


bool mouseRButtonPressed = false;
bool mouseLButtonPressed = false;

Packet Player::Break(World& world, Renderer& r)
{
	Packet packet = { 0 };
	packet.type = -1;
	Chunk* c = world.GetChunkFromWorldPosition(this->position.x, this->position.y);
	if (c == nullptr) return packet;
	Block* b = FindClosestBlock(&c);
	if (b == NULL) return packet;
	r.SetSelectedCube(b->GetPosition());


	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
		if (!mouseRButtonPressed) {
			mouseRButtonPressed = true;
			glm::vec3 pos = b->GetLocalPosition();
			c->DestroyBlock(pos.x, pos.y, pos.z);
			//c->UpdateMasks();
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
		}
		glm::vec3 global_position = b->GetPosition();
		packet.x = global_position.x;
		packet.y = global_position.y;
		packet.z = global_position.z;
		packet.type = BLOCK_DESTRUCTION_PACKET;
		return packet;
	}
	else {
		mouseRButtonPressed = false;
	}
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
		if (!mouseLButtonPressed) {
			mouseLButtonPressed = true;
			glm::vec3 pos = b->GetPosition();
			glm::vec3 ray_direction = GetRayDirection(*this->cam);
			glm::vec3 ray_origin = this->position;
			glm::vec3 face = IntersectCube(ray_origin, ray_direction, pos, 0.5f);
			
			glm::vec3 l_pos = b->GetLocalPosition() + face;
			if (l_pos.x >= CHUNK_SIZE_X) {
				if (c->n_right && *c->n_right) {
					c = *c->n_right;
				}
			}
			else if (l_pos.x < 0) {
				if (c->n_left && *c->n_left) c = *c->n_left;
			}
			if (l_pos.y >= CHUNK_SIZE_Y) {
				if (c->n_front && *c->n_front) c = *c->n_front;
			}
			else if (l_pos.y < 0) {
				if (c->n_back && *c->n_back) c = *c->n_back;
			}
			int x = l_pos.x;
			int y = l_pos.y;
			World::WorldToLocalChunk(x, y);
			l_pos.x = x;
			l_pos.y = y;

			c->BuildBlock((int)l_pos.x, (int)l_pos.y, (int)l_pos.z, selected);
			c->UpdateNeighbors();
			c->UpdateMasks();
			c->UpdateInstances();
			glm::vec3 global_position = b->GetPosition();
			packet.x = global_position.x;
			packet.y = global_position.y;
			packet.z = global_position.z;
			packet.w = selected;
			packet.type = BLOCK_PLACEMENT_PACKET;
			return packet;
		}
	}
	else {
	mouseLButtonPressed = false;
	}
}
