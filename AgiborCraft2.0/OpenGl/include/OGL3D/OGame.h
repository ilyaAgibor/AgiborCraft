#pragma once
#include "OGL3D/AgiborNet.h"
#include <ctime>
#include <queue>
#include <GLEW/glew.h>
#include <iostream>
#include <thread>
#include <atomic>
#include "OGL3D/OWindow.h"
#include "OGL3D/Shader.h"
#include "OGL3D/Texture.h"
#include "OGL3D/TextureArray.h"
#include "OGL3D/IndexBuffer.h"
#include "OGL3D/VertexBuffer.h"
#include "OGL3D/VertexBufferLayout.h"
#include "OGL3D/Renderer.h"
#include "OGL3D/MVP.h"
#include "OGL3D/Player.h"
#include "OGL3D/Block.h"
#include "OGL3D/Chunk.h"
#include "OGL3D/World.h"

#define PassPacket() packet.id, packet.x, packet.y, packet.z, packet.w

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
	OGame();
	~OGame();

	double CalculateDeltaTime();
	void Run();
	void Quit();

private:
	void PacketReceiver();
	void PacketProccesor();
	void PacketHandler();
	void PlayerJoin(int id, float x = 0, float y = 0, float z = 0, float w = 0);
	void PlayerLeave(int id);
	void UpdatePlayerById(int id, float x = 0, float y = 0, float z = 0, float w = 0);

protected:
	std::vector<PlayerEntity> players;
	std::queue<Packet> packets_queue;
	std::vector<Packet> received_packets; 
	std::thread receiver_thread;
	std::thread processor_thread;
	std::atomic<bool> m_isRunning = ATOMIC_VAR_INIT(true);
	OWindow* m_display = nullptr;
	AgiborNet network;
	HWND l_hwnd = nullptr;
	HDC hDC = nullptr;
	HGLRC opctx = nullptr;
	int player_id = 0;

	bool EnableOpenGL();
	bool DisableOpenGL();
};

