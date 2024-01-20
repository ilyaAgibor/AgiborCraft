#include "OGL3D/Player.h"


Player::Player(MVP* mvp, float ac, float friction) : mvp(mvp), acceleration(ac), friction(friction)
{
	this->selected = SAND;
	this->position = glm::vec3(48,48,70);
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
	if (GetKeyState('8') < 0) {
		this->selected = GRASS;
	}
}

	

void Player::Controls(float delta_time)
{
	// Move the camera based on the delta time
	this->cam->MoveCamera(delta_time);
	constexpr float half = glm::radians(45.0f);
	// Handle vertical movement (up and down)
	if (GetKeyState('W') < 0) {
		// Increase vertical velocity if it's below the maximum speed
			AddForce({0,speed,0});
	}
	else if (GetKeyState('S') < 0) {
		// Decrease vertical velocity if it's above the negative maximum speed
			AddForce({ 0,-speed,0 });
	}
	// Handle horizontal movement (left and right)
	if (GetKeyState('A') < 0) {
		// Decrease horizontal velocity if it's above the negative maximum speed
			AddForce({ -speed,0,0 });
	}
	else if (GetKeyState('D') < 0) {
		// Increase horizontal velocity if it's below the maximum speed
			AddForce({ speed,0,0 });
	}
	// Handle jumping and crouching
	if (GetKeyState(VK_SPACE) < 0) {
		if (godmode) {
			// Increase vertical velocity if in god mode and jump key is pressed
				AddForce({ 0,0, speed });
		}
		else if (is_grounded)
		{
			// Enable jumping if grounded and jump key is pressed
			is_jumping = true;
		}
	}
	else if ((GetKeyState(VK_SHIFT) < 0) && godmode) {
		// Decrease vertical velocity if in god mode and crouch key is pressed
			AddForce({ 0,0,-speed });
	}
	else
	{
		if (is_grounded && !godmode && !is_jumping) {
			// Apply friction to vertical velocity when no input is detected and not in god mode
			this->velocity.z *= 0;
			// Set vertical velocity to zero if it's close to zero
		}
	}
	
	physics(delta_time);

	this->position.x += this->velocity.x * std::sin(-glm::radians(this->cam->GetAngle().y - 90)) * delta_time;
	this->position.y += this->velocity.x * std::cos(-glm::radians(this->cam->GetAngle().y - 90)) * delta_time;
	this->position.x += this->velocity.y * std::sin(-glm::radians(this->cam->GetAngle().y)) * delta_time;
	this->position.y += this->velocity.y * std::cos(-glm::radians(this->cam->GetAngle().y)) * delta_time;
	this->position.z += this->velocity.z * delta_time;
	// Translate the model-view-projection matrix based on the calculated position
	this->mvp->Translate(glm::vec3(-this->position.x, -this->position.y, -this->position.z));
	// Handle player input (not provided in the code snippet)
	this->Input();
	acceleration = glm::vec3(0);
}

void Player::physics(float delta_time)
{
	if (fabs(this->velocity.x) <= 0.01f) this->velocity.x = 0;
	if (fabs(this->velocity.y) <= 0.01f) this->velocity.y = 0;
	if (fabs(this->velocity.z) <= 0.01f) this->velocity.z = 0;

	glm::vec3 xy_vel = { velocity.x, velocity.y, 0 };
	float vertical_speed = glm::length(xy_vel);
	if (vertical_speed >= max_vel)
	{
		float norm = max_vel / vertical_speed;
		xy_vel *= norm;
		velocity.x = xy_vel.x;
		velocity.y = xy_vel.y;
	}
	

	float f_max = s_friction * gravity;
	if (is_grounded) {
		if (velocity.x == 0 && velocity.y == 0) {
			//static friction
			if (glm::length(acceleration) <= f_max)
				acceleration = glm::vec3(0);
		}
		else
		{
			acceleration += -glm::normalize(velocity) * f_max;
		}
	}

	// Apply velocity based on delta time to calculate the new position
	this->velocity += acceleration * delta_time;
}

void Player::AddForce(glm::vec3 f)
{
	acceleration += f;
}

bool Player::CheckGrounded(World& world, Block** b)
{
	int z = 0;
	for (z = (int)position.z; z > 0; z--) {
		if (*b = world.CheckBlock(position.x, position.y, z))
			break;
	}
	bool ret = (position.z - z) < this->height;
	return ret;
}

typedef struct _CollisionInfo {
	bool collidingX;
	bool collidingY;
	glm::vec2 penetration;
} CollisionInfo;

CollisionInfo CheckCollision(glm::vec3 player_position, float player_radius ,glm::vec3 block_position, float block_size) {
	CollisionInfo info;
	info.collidingX = false;
	info.collidingY = false;

	float xDiff = std::abs(player_position.x - block_position.x);
	float yDiff = std::abs(player_position.y - block_position.y);

	if (xDiff > (block_size * 0.5f + player_radius)) {
		info.penetration = glm::vec2(0.0f);
		return info;
	}
	if (yDiff > (block_size * 0.5f + player_radius)) {
		info.penetration = glm::vec2(0.0f);
		return info;
	}

	if (xDiff <= block_size * 0.5f) {
		info.collidingX = true;
		info.penetration.x = (block_size * 0.5f + player_radius) - xDiff;
	}
	if (yDiff <= block_size * 0.5f) {
		info.collidingY = true;
		info.penetration.y = (block_size * 0.5f + player_radius) - yDiff;
	}

	return info;
}

bool Player::Collisions(World& world)
{
	// Loop through two iterations for vertical and horizontal collisions
	for (int i = 0; i < 2; i++) {
		// Loop through neighboring blocks in a 3x3 area
		for (int x_offset = -1; x_offset <= 1; x_offset++) {
			for (int y_offset = -1; y_offset <= 1; y_offset++) {
				// Skip the player's own position
				if (x_offset == 0 && y_offset == 0) continue;
				// Check the block at the current offset from the player's calculated position
				Block* b = world.CheckBlock(position.x + x_offset, position.y + y_offset, position.z - 1 * i);
				if (b == nullptr) continue;
				// Get the position of the block and the player
				glm::vec3 block_position = b->GetPosition() + glm::vec3(0.5f);
				glm::vec3 player_position = this->position;
				player_position.z -= 1 * i;
				// Check for collision between the player and the block
				CollisionInfo info = CheckCollision(player_position, this->thickness, block_position, 1);
				// Update collision flags based on the collision information
				colliding_y = info.collidingX;
				colliding_x = info.collidingY;
				// Handle collision along the Y-axis
				if (info.collidingY) {
					float sign = (position.x < block_position.x) ? -1.0f : 1.0f;
					this->position.x += sign * info.penetration.y;
					this->velocity.x = 0;
				}
				// Handle collision along the X-axis
				if (info.collidingX) {
					float sign = (position.y < block_position.y) ? -1.0f : 1.0f;
					this->position.y += sign * info.penetration.x;
					this->velocity.y = 0;
				}
			}
		}
	}
	// Return true if there is a collision along either the X-axis or Y-axis
	return colliding_x || colliding_y;
}


void Player::Logic(World& world, float delta_time)
{
	Collisions(world);
	Block* b;
	is_grounded = CheckGrounded(world, &b);
	if (!godmode && !is_grounded) {
		//printf("gravity %f\n", position.z);
		AddForce({ 0,0, -gravity });
	}
	if (is_jumping && is_grounded && acceleration.z == 0) {
		velocity.z = 6;
		is_jumping = false;
		is_grounded = false;
	}
	if (!is_jumping && is_grounded && b) {
		position.z = b->GetLocalPosition().z + height;
	}

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

glm::vec3 GetRayDirection(const Camera& camera) {
	double pitch = glm::radians(camera.GetAngle().x);
	double yaw = glm::radians(camera.GetAngle().y);

	double x = std::sin(pitch) * std::cos(yaw);
	double y = std::sin(pitch) * std::sin(yaw);
	double z = std::cos(pitch);
	return glm::vec3{ -y, x, -z };
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
		x = (int)(ray_end.x);
		y = (int)(ray_end.y);
		z = (int)(ray_end.z);
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

glm::vec3 IntersectCube(const glm::vec3& ray_origin, const glm::vec3& ray_direction, const glm::vec3& position) {
	glm::vec3 _inv_dir = { 1.0f / ray_direction.x, 1.0f / ray_direction.y, 1.0f / ray_direction.z };
	glm::vec3 _bbox_min = position;
	glm::vec3 _bbox_max = { _bbox_min.x + 1, _bbox_min.y + 1, _bbox_min.z + 1 };

	float tx1 = (_bbox_min.x - ray_origin.x) * _inv_dir.x;
	float tx2 = (_bbox_max.x - ray_origin.x) * _inv_dir.x;

	float tmin = std::min(tx1, tx2);
	float tmax = std::max(tx1, tx2);

	float ty1 = (_bbox_min.y - ray_origin.y) * _inv_dir.y;
	float ty2 = (_bbox_max.y - ray_origin.y) * _inv_dir.y;

	tmin = std::max(tmin, std::min(ty1, ty2));
	tmax = std::min(tmax, std::max(ty1, ty2));


	float tz1 = (_bbox_min.z - ray_origin.z) * _inv_dir.z;
	float tz2 = (_bbox_max.z - ray_origin.z) * _inv_dir.z;

	tmin = std::max(tmin, std::min(tz1, tz2));
	tmax = std::min(tmax, std::max(tz1, tz2));

	glm::vec3 _pos = {};
	_pos.x = (tmin * ray_direction.x) + ray_origin.x;
	_pos.y = (tmin * ray_direction.y) + ray_origin.y;
	_pos.z = (tmin * ray_direction.z) + ray_origin.z;

	if (abs(_pos.x - position.x) < 0.00001f) //left (-x) face
	{
		//return left face enum, or return the calcualtedposition
		//i myself return return the origin for a new block to be made
		return {-1,0,0};
	}

	if (abs(_pos.x - position.x) > 0.99990) //right (+x) face
	{
		return {1,0,0};
	}

	if (abs(_pos.y - position.y) < 0.00001f) // bot (-y) face
	{
		return {0,-1,0};
	}

	if (abs(_pos.y - position.y) > 0.99990f) // top (+y) face
	{
		return {0,1,0};
	}

	if (abs(_pos.z - position.z) < 0.00001f) // front (-z) face
	{
		return {0,0,-1};
	}

	if (abs(_pos.z - position.z) > 0.99990f) // back (+z) face
	{
		return {0,0,1};
	}
}




Packet Player::Break(World& world, Renderer& r)
{
	static bool mouseRButtonPressed = false;
	static bool mouseLButtonPressed = false;
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

			glm::vec3 global_position = b->GetPosition();
			packet.x = global_position.x;
			packet.y = global_position.y;
			packet.z = global_position.z;
			packet.w = 0;
			packet.type = BLOCK_DESTRUCTION_PACKET;
			return packet;
		}
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
			glm::vec3 face = IntersectCube(ray_origin, ray_direction, pos);
			
			glm::vec3 l_pos = b->GetLocalPosition() + face;
			glm::vec3 g_pos = b->GetPosition() + face;
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
			packet.x = g_pos.x;
			packet.y = g_pos.y;
			packet.z = g_pos.z;
			packet.w = selected;
			packet.type = BLOCK_PLACEMENT_PACKET;
			return packet;
		}
	}
	else {
	mouseLButtonPressed = false;
	}
}
