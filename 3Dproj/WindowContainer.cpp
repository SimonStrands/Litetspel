#include "WindowContainer.h"

WindowContainer::WindowContainer()
{
	this->mouse = new Mouse();
	this->keyboard = new Keyboard();

	static bool raw_input_initialized = false;
	if (raw_input_initialized == false)
	{
		RAWINPUTDEVICE rid;

		rid.usUsagePage = 0x01; //Mouse
		rid.usUsage = 0x02;
		rid.dwFlags = 0;
		rid.hwndTarget = NULL;

		if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
		{
			std::cout << "error register raw input device" << std::endl;
			exit(-1);
		}

		raw_input_initialized = true;
	}
}

WindowContainer::~WindowContainer()
{
	delete mouse;
	delete keyboard;
}

LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYUP:
	{
		unsigned char ch = static_cast<unsigned char>(wParam);
		keyboard->onKeyReleased(ch);
		return 0;
	}
	case WM_KEYDOWN:
	{
		unsigned char ch = static_cast<unsigned char>(wParam);
		keyboard->onKeyDown(ch);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse->onMouseMove(x, y);
		return 0; 
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse->onLeftPressed(x, y);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse->onRightPressed(x, y);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse->onMiddlePressed(x, y);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse->onLeftReleased(x, y);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse->onRightReleased(x, y);
		return 0;
	}
	case WM_MBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse->onMiddleReleased(x, y);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			mouse->onWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			mouse->onWheelDown(x, y);
		}
		return 0;
	}
	case WM_INPUT:
	{
		if (mouse->getMouseActive()) {
			UINT dataSize;
			GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER)); //Need to populate data size first

			if (dataSize > 0)
			{
				std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);
				if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
				{
					RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
					if (raw->header.dwType == RIM_TYPEMOUSE)
					{
						//std::cout << raw->data.mouse.lLastX << ":" << raw->data.mouse.lLastY << std::endl;
						mouse->onMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
					}
				}
			}
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam); //Need to call DefWindowProc for WM_INPUT messages
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

RenderWindow& WindowContainer::getRenderWindow()
{
	return this->render_window;
}

Mouse*& WindowContainer::getMouse()
{
	return this->mouse;
}

Keyboard*& WindowContainer::getKeyboard()
{
	return this->keyboard;
}


/*
Window::WindowClass Window::WindowClass::wndClass;

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

Window::WindowClass::WindowClass() noexcept
	:hInst(GetModuleHandle(nullptr))
{
	const wchar_t CLASS_NAME[] = L"WinClass";

	WNDCLASS wc = {};
	wc.lpfnWndProc = HandleMsgSetup;
	wc.hInstance = GetInstance();
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);
}

Window::WindowClass::~WindowClass()
{
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}

switch (msg)
{
case WM_INPUT:
{
	//std::cout << this->height << std::endl;
	//if (mouse != nullptr) {
	UINT datasize;
	// first get the size of the input data
	if (GetRawInputData(
		reinterpret_cast<HRAWINPUT>(lParam),
		RID_INPUT,
		nullptr,
		&datasize,
		sizeof(RAWINPUTHEADER)) == -1)
	{
		// bail msg processing if error
		break;
	}
	std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(datasize);
	while (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &datasize, sizeof(RAWINPUTHEADER) == datasize)) {
		RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
		if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			//mouse->onMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
			std::cout << raw->data.mouse.lLastX << std::endl;
			std::cout << raw->data.mouse.lLastY << std::endl;
		}

	}
	//}


}
default:
	break;
}

return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Window::SetMouse(Mouse* mouse)
{
	this->mouse = mouse;
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// retrieve ptr to window instance
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window instance handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window instance
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		// forward message to window instance handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::Window(int width, int height, HINSTANCE hInstance, HWND& wnd, Mouse*& mouse) :
	width(width),
	height(height)
{
	this->mouse = mouse;
	// calculate window size based on desired client region size
	const wchar_t CLASS_NAME[] = L"WinClass";

	wnd = CreateWindowEx(
		0, CLASS_NAME, L"hej", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, width, height, nullptr, nullptr, hInstance, nullptr
	);
	// check for error
	if (wnd == nullptr)
	{
		std::cout << "error" << std::endl;
	}

	ShowWindow(wnd, SW_SHOWDEFAULT);

	ImGui_ImplWin32_Init(wnd);

	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01; // mouse page
	rid.usUsage = 0x02; // mouse usage
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
	{
		std::cout << "error with raw mouse" << std::endl;
	}
}

Window::~Window()
{
	ImGui_ImplWin32_Shutdown();
}
*/

