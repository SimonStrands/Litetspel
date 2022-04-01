#pragma once
#include "Mouse.h"
#include "RenderWindow.h"

class WindowContainer
{
public:
	WindowContainer();
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	RenderWindow& getRenderWindow();
	Mouse*& getMouse();
protected:
	RenderWindow render_window;
	//KeyboardClass keyboard;
	Mouse* mouse;
};