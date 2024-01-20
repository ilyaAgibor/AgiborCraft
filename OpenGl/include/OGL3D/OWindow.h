#pragma once
#include "OGL3D/OGame.h"
#include <Windows.h>
#include "glm/glm.hpp"
#include "GLEW/glew.h"

class OWindow
{
public:
	OWindow();
	~OWindow();

	void onDestroy();
	bool isClosed();
	HWND GetWindowHanlderPrt();
	bool isPaused() { return this->paused; }
	void setPaused(bool val) {
		this->paused = val;
	}
	glm::vec2 window_size;
	bool changed_size = false;
private:
	bool paused = 0;
	void* m_handle = nullptr;
};

