#pragma once
#include "Mouse.h"
#include "Keyboard.h"
#include "RenderWindow.h"

class WindowContainer
{
public:
	WindowContainer();
	virtual ~WindowContainer();
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	RenderWindow& getRenderWindow();
	Mouse*& getMouse();
	Keyboard*& getKeyboard();
protected:
	RenderWindow render_window;
	Keyboard* keyboard;
	Mouse* mouse;
};