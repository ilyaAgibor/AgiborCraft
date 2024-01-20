#pragma once
#include "AgiborNet.h"
#include <ctime>
#include <queue>
#include <GLEW/glew.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <random>
#include "OWindow.h"
#include "Shader.h"
#include "Texture.h"
#include "TextureArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "MVP.h"
#include "Player.h"
#include "Block.h"
#include "Chunk.h"
#include "World.h"

#define PassPacket(p) p.id, p.x, p.y, p.z, p.w

typedef struct _PlayerEntity {
	int id;
	float x;
	float y;
	float z;
	float w;
} PlayerEntity;

class OWindow;
class OGame
{
public:
	OGame(int id);
	~OGame();

	double CalculateDeltaTime();
	void Run();
	void Quit();

private:
	void PacketReceiver();
	void PacketProccesor();
	void PacketSender();
	void PacketHandler();
	void PushPacket(Packet p);
	Packet PopPacket();
	void PlayerJoin(int id, float x = 0, float y = 0, float z = 0, float w = 0);
	void PlayerLeave(int id);
	void UpdatePlayerById(int id, float x = 0, float y = 0, float z = 0, float w = 0);
	void PlayerBreakBlock(int id, float x = 0, float y = 0, float z = 0, float w = 0);
	void PlayerPlacementBlock(int id, float x = 0, float y = 0, float z = 0, float w = 0);
	void GenerateWorld(unsigned int seed = 0);
protected:
	unsigned int world_seed = 0;
	std::vector<PlayerEntity> players;
	std::queue<Packet> packets_queue;
	std::queue<Packet> send_queue;
	std::vector<Packet> received_packets;
	void* raw_data = nullptr;
	unsigned int raw_size;
	std::thread sender_thread;
	std::thread receiver_thread;
	std::thread processor_thread;
	std::atomic<bool> m_isRunning = ATOMIC_VAR_INIT(true);
	std::mutex send_mutex;
	std::mutex receiving_packet_mutex;
	OWindow* m_display = nullptr;
	AgiborNet network;
	HWND l_hwnd = nullptr;
	HDC hDC = nullptr;
	HGLRC opctx = nullptr;
	int player_id = 0;
	World* p_world;

	bool EnableOpenGL();
	bool DisableOpenGL();
};

