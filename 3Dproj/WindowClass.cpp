#include "imgui_impl_win32.h"
#include "WindowClass.h"


bool Window::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	return this->render_window.Initialize(this, hInstance, window_title, window_class, width, height);
}

bool Window::ProcessMessages()
{
	return this->render_window.ProcessMessages();
}

void Window::Update()
{
	//nothing here yet

	//read mouse
}
