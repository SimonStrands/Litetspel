#pragma once
#include <Windows.h>
#include <string>
#include <iostream>

class WindowContainer;

class RenderWindow
{
public:
	bool Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	bool ProcessMessages();
	~RenderWindow();
	HWND &getHandle();
	HINSTANCE& gethInstance();
private:
	void RegisterWindowClass();
	HWND handle = NULL; //Handle to this window
	HINSTANCE hInstance = NULL; //Handle to application instance
	int width = 0;
	int height = 0;
};
