 #include "OGame.h"
#include "imGui/imgui.h"
#include "imGui/imgui_impl_win32.h"
#include "imGui/imgui_impl_opengl3.h"
#include "InstanceBuffer.h"

OGame::OGame(int id)
{
	this->m_display = new OWindow();
	this->l_hwnd = this->m_display->GetWindowHanlderPrt();
	
	this->EnableOpenGL();
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, "Status: Using OpenGL %s\n", glGetString(GL_VERSION));

	this->player_id = id;
	printf("Player Id: %d\n", id);
}

OGame::~OGame()
{
	this->DisableOpenGL();
	delete this->m_display;
	delete this->p_world;
}


double OGame::CalculateDeltaTime()
{
	static LARGE_INTEGER previous_time = {};
	static LARGE_INTEGER frequency = {};
	if (previous_time.QuadPart == 0) {
		QueryPerformanceCounter(&previous_time);
		QueryPerformanceFrequency(&frequency);
	}
	LARGE_INTEGER current_time;
	QueryPerformanceCounter(&current_time);
	double delta_time = (current_time.QuadPart - previous_time.QuadPart) / (double)frequency.QuadPart;
	previous_time = current_time;
	return delta_time;
}




void OGame::PacketReceiver() {
	while (this->m_isRunning) {
		Packet rec = this->network.Recive();
		if (rec.type == -1)
			continue;
		this->receiving_packet_mutex.lock();
		this->packets_queue.push(rec);
		this->receiving_packet_mutex.unlock();
	}
}

void OGame::PacketProccesor() {
	while (this->m_isRunning) {
		this->receiving_packet_mutex.lock();
		if (!this->packets_queue.empty()) {
			Packet p = this->packets_queue.front();
			this->packets_queue.pop();
			this->received_packets.push_back(p);
		}
		this->receiving_packet_mutex.unlock();
	}
}

void OGame::PacketSender()
{
	while (this->m_isRunning) {
		if (!this->send_queue.empty()) {
			Packet p = PopPacket();
			network.Send(p);
		}
	}
}


void OGame::PlayerJoin(int id, float x, float y, float z, float w)
{
	for (auto player : this->players) {
		if (player.id == id) {
			printf("Player %d is already logged in!\n", id);
			return;
		}
	}
	this->players.push_back({id, x, y, z, w});
	printf("Player %d joined!\n", id);
	if ((unsigned int)x && !this->world_seed) {
		GenerateWorld((unsigned int)x);
		this->world_seed = (unsigned int)x;
		printf("world was excisted %u\n", (unsigned int)x);
	}

	//send packet back to player to let them know you excist
	if(p_world)
		PushPacket({ PLAYER_CONNECTION_PACKET, this->player_id, (float)p_world->GetWorldSeed(), 0, 0, 0});
	else
		PushPacket({ PLAYER_CONNECTION_PACKET, this->player_id, 0, 0, 0, 0 });
}


void OGame::PlayerLeave(int id)
{
	for (int i = 0; i < this->players.size(); i++) {
		if (this->players[i].id == id) {
			this->players.erase(this->players.begin()+i);
			printf("Player %d has left!\n", id);
			return;
		}
	}
	printf("no player with id: %d\n", id);
}

void OGame::UpdatePlayerById(int id, float x, float y, float z, float w)
{
	for (auto& player : this->players) {
		if (player.id == id) {
			player.x = x;
			player.y = y;
			player.z = z;
			player.w = w;
			return;
		}
	}
}

void OGame::PlayerBreakBlock(int id, float x, float y, float z, float w)
{
	this->p_world->DestroyBlock(x, y, z);
}

void OGame::PlayerPlacementBlock(int id, float x, float y, float z, float w)
{
	this->p_world->PlaceBlock(x,y,z,w);
}

void OGame::GenerateWorld(unsigned int seed)
{
	this->p_world = new World(seed);
}


void OGame::PacketHandler() {
	for (auto packet : this->received_packets) {
		switch (packet.type)
		{
		case PLAYER_CONNECTION_PACKET:
			this->PlayerJoin(PassPacket(packet));
			break;
		case PLAYER_POS_PACKET:
			this->UpdatePlayerById(PassPacket(packet));
			break;
		case PLAYER_DISCONNECTION_PACKET:
			this->PlayerLeave(packet.id);
			break;
		case BLOCK_DESTRUCTION_PACKET:
			this->PlayerBreakBlock(PassPacket(packet));
			break;
		case BLOCK_PLACEMENT_PACKET:
			this->PlayerPlacementBlock(PassPacket(packet));
			break;
		default:
			break;
		}
		this->received_packets.pop_back();
	}
}

void OGame::PushPacket(Packet p)
{
	this->send_mutex.lock();
	this->send_queue.push(p);
	this->send_mutex.unlock();
}

Packet OGame::PopPacket()
{
	this->send_mutex.lock();
	Packet p = this->send_queue.front();
	this->send_queue.pop();
	this->send_mutex.unlock();
	return p;
}


void OGame::Run()
{

	MSG msg;
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	MVP mvp(75.0f);
	
	Player player(&mvp, 5, 1.0f);
	
	GLenum error = glGetError();

	Renderer renderer;
	renderer.Set();



	int texture_count = 8;
	TextureArray textures(texture_count, 16, 16, 4);
	textures.AddTexture("../res/textures/dirt.png");	
	textures.AddTexture("../res/textures/stone.png");
	textures.AddTexture("../res/textures/sand.png");
	textures.AddTexture("../res/textures/diamond.png");
	textures.AddTexture("../res/textures/coal.png");
	textures.AddTexture("../res/textures/white.png");
	textures.AddTexture("../res/textures/red.png");
	textures.AddTexture("../res/textures/grass.png");

	textures.Bind(0);
	renderer.SetTextureSlot(0);

	this->receiver_thread = std::thread(&OGame::PacketReceiver, this);
	this->processor_thread = std::thread(&OGame::PacketProccesor, this);
	this->sender_thread = std::thread(&OGame::PacketSender, this);
	
	//enter to local network communication
	PushPacket({ PLAYER_CONNECTION_PACKET, this->player_id, 0, 0, 0, 0 });
	//PlayerJoin(1, 0, 0, 0, 0);
	Sleep(100);
	PacketHandler();
	if (!world_seed)
		GenerateWorld();

	World& world = *this->p_world;


	float f = 0;
	while (m_isRunning && !m_display->isClosed()) {
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (this->m_display->changed_size) {
				mvp.ChangePrespectiveRatio(this->m_display->window_size.x / this->m_display->window_size.y);
				this->m_display->changed_size = false;
			}
			double d = CalculateDeltaTime();
			std::string fps = "AgiborCraft 2.0 - " + std::to_string(1 / d) + " fps";
			SetWindowTextA(this->m_display->GetWindowHanlderPrt(), fps.c_str());

			PacketHandler();

			mvp.PushMatrix();
			renderer.BackGround(0.1f, 0.3f, 0.8f, 0.0f);
			renderer.Clear();


			player.Logic(world, d);
			
			if (!this->m_display->isPaused() && GetForegroundWindow() == this->m_display->GetWindowHanlderPrt()) {
				player.Controls(d);
				ShowCursor(FALSE);
			}
			else {
				ShowCursor(TRUE);
			}



			if (this->players.size() > 0) {
				renderer.BindPlayer();
				renderer.UpdateMVP(mvp);
				for(int i = 0; i < this->players.size(); i++)
					renderer.DrawPlayer(this->players[i].x, this->players[i].y, this->players[i].z-1.8, this->players[i].w);
			}

			glm::vec3 player_position = player.GetPosition();
			renderer.BindBlock();
			Packet break_packet = player.Break(world, renderer);
			renderer.UpdateMVP(mvp);
			world.Draw(renderer, mvp);

			mvp.PopMatrix();

			//send position packets
			PushPacket({PLAYER_POS_PACKET, this->player_id, player_position.x, player_position.y, player_position.z, player.GetDirection().y});

			//send break/placement packet
			if (break_packet.type != -1) {
				break_packet.id = this->player_id;
				network.Send(break_packet);
			}

			SwapBuffers(this->hDC);
			f += 0.01f;
		}
	}
	PushPacket({ PLAYER_DISCONNECTION_PACKET, this->player_id,0 ,0 ,0 , 0 });

	this->Quit();
	this->receiver_thread.join();
	this->processor_thread.join();
	this->sender_thread.join();

}

void OGame::Quit()
{
	m_isRunning = false;
	std::cout << "Program Finished Running\n";
}


bool OGame::EnableOpenGL() {
	
	this->hDC = GetDC(this->l_hwnd);
	int pFormat;
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	pFormat = ChoosePixelFormat(this->hDC, &pfd);
	if (pFormat == 0) {
		std::cout << "Coud'nt Find Pixel Format!\n";
		this->Quit();
		return 0;
	}
	SetPixelFormat(this->hDC, pFormat, &pfd);
	this->opctx = wglCreateContext(this->hDC);
	wglMakeCurrent(this->hDC, this->opctx);
	return 1;
}

bool OGame::DisableOpenGL() {
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(this->opctx);
	ReleaseDC(this->l_hwnd, this->hDC);
	return 1;
}