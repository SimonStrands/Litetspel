#pragma once
#include "WindowContainer.h"

class Window : public WindowContainer
{
public:
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	bool ProcessMessages();
	void Update();
	void HideCoursor();
	void ShowCoursor();
private:
	void confineCoursor();
};