#include "WindowHelper.h"
#include <iostream>
#include "imgui_impl_win32.h"
//git
LRESULT CALLBACK WindowProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(wnd, msg, wParam, lParam))
	{
		return true;
	}
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}
	return DefWindowProc(wnd, msg, wParam, lParam);
}

bool setUpWindow(HINSTANCE hInstance, UINT WIDTH, UINT HEIGHT, int nCmdShow, HWND& wnd)
{
	const wchar_t CLASS_NAME[] = L"WinClass";

	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);
																	//CW_USEDEFAULT
	wnd = CreateWindowEx(0, CLASS_NAME, L"hej", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, WIDTH, HEIGHT, nullptr, nullptr, hInstance, nullptr);

	if (wnd == nullptr) {
		std::cout << "cant create window" << std::endl;
		return false;
	}

	ShowWindow(wnd, nCmdShow);


	return true;
}

void shutDownWindow()
{
	ImGui_ImplWin32_Shutdown();
}




