#include "OGL3D/OWindow.h"
#include <assert.h>
#include <iostream>



LRESULT CALLBACK WndProc1(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_DESTROY: {
		OWindow* window = (OWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onDestroy();
		break;
	}
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			OWindow* window = (OWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			if (window->isPaused()) {
				window->setPaused(false);
			}
			else {
				window->setPaused(true);
			}
			
		}
		break;
	case WM_SIZE: {
		int width = LOWORD(lParam);
		int height = HIWORD(lParam);
		OWindow* window = (OWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->window_size = glm::vec2(width, height);
		window->changed_size = true;
		glViewport(0, 0, width, height);
		break;
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return NULL;
}

void OWindow::onDestroy() {
	this->m_handle = nullptr;
}

bool OWindow::isClosed() {
	return !this->m_handle;
}

OWindow::OWindow()
{
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = L"MyWindowClass";
	wc.lpfnWndProc = WndProc1;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&wc);

	m_handle = CreateWindowEx(0, L"MyWindowClass", L"My Window", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1200, 800, NULL, NULL, NULL, NULL);

	if (m_handle == NULL)
	{
		fprintf(stderr, "coudn't create window!\n");
		return;
	}

	SetWindowLongPtr((HWND)m_handle, GWLP_USERDATA, (LONG_PTR)this);

	ShowWindow((HWND)m_handle, SW_SHOW);
	UpdateWindow((HWND)m_handle);
}

OWindow::~OWindow()
{
	DestroyWindow((HWND)m_handle);
}

HWND OWindow::GetWindowHanlderPrt() {

	return (HWND)this->m_handle;
};
