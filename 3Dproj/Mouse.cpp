#include "Mouse.h"
#include <iostream>

//use sfml
Mouse::Mouse()
{
	setUpMouse();
	mouseSense = 0.6;
	mouse_active = false;
	once = false;
	x = y = 0;
	leaftIsDown = rightIsDown = midIsDown = false;
}

Mouse::~Mouse()
{
}

void Mouse::updateMouse(MSG msg)
{
	int x = LOWORD(msg.lParam);
	int y = HIWORD(msg.lParam);
	switch (msg.message) {
	case WM_MOUSEMOVE:
		onMouseMove(x, y);
		break;
	case WM_CHAR:
		break;
	} 
}

bool Mouse::IsLeftDown()
{
	return this->leaftIsDown;
}

bool Mouse::isMiddleDown()
{
	return this->midIsDown;
}

bool Mouse::isRightDown()
{
	return this->rightIsDown;
}

MousePoint Mouse::getPos()
{
	return MousePoint({x,y});
}

int Mouse::getPosX()
{
	return x;
}

int Mouse::getPosY()
{
	return y;
}

bool Mouse::EventBufferEmpty()
{
	return mouseBuffer.empty();
}

mouseEvent Mouse::ReadEvent()
{
	if (EventBufferEmpty()) {
		return mouseEvent();
	}
	mouseEvent e = mouseBuffer.front();
	mouseBuffer.pop();
	return e;
}

void Mouse::changeSense(float newSence)
{
	this->mouseSense = newSence;
}


float Mouse::getSense()
{
	return mouseSense;
}

bool Mouse::getMouseActive()
{
	return mouse_active;
}

void Mouse::onLeftPressed(int x, int y)
{
	this->leaftIsDown = true;
	mouseEvent m(mouseEvent::EventType::LPress, x, y);
	mouseBuffer.push(m);
}

void Mouse::onLeftReleased(int x, int y)
{
	this->leaftIsDown = false;
	mouseEvent m(mouseEvent::EventType::LRelease, x, y);
	mouseBuffer.push(m);
}

void Mouse::onRightPressed(int x, int y)
{
	this->rightIsDown = true;
	mouseEvent m(mouseEvent::EventType::RPress, x, y);
	mouseBuffer.push(m);
}

void Mouse::onRightReleased(int x, int y)
{
	this->rightIsDown = false;
	mouseEvent m(mouseEvent::EventType::RRelease, x, y);
	mouseBuffer.push(m);
}

void Mouse::onMiddlePressed(int x, int y)
{
	this->midIsDown = true;
	mouseEvent m(mouseEvent::EventType::MPress, x, y);
	mouseBuffer.push(m);
}

void Mouse::onMiddleReleased(int x, int y)
{
	this->midIsDown = false;
	mouseEvent m(mouseEvent::EventType::MRelease, x, y);
	mouseBuffer.push(m);
}

void Mouse::onWheelUp(int x, int y)
{
	mouseEvent m(mouseEvent::EventType::WheelUp, x, y);
	mouseBuffer.push(m);
}

void Mouse::onWheelDown(int x, int y)
{
	mouseEvent m(mouseEvent::EventType::WheelDown, x, y);
	mouseBuffer.push(m);
}

void Mouse::onMouseMove(int x, int y)
{
	mouseEvent m(mouseEvent::EventType::Move, x, y);
	mouseBuffer.push(m);
}

void Mouse::activateMouse()
{
	if (GetKeyState(VK_TAB) & 0x8000) {
		if (!once) {
			once = true;
			if (mouse_active) {
				mouse_active = false;
			}
			else {
				mouse_active = true;
			}
		}
	}
	else {
		once = false;
	}
	if (GetKeyState(VK_ESCAPE) & 0x8000) {
		mouse_active = false;
	}
}

/*Mouse event*/
mouseEvent::mouseEvent():
	type(EventType::INVALID),
	x(0),
	y(0)
{}

mouseEvent::mouseEvent(const EventType type, const int x, const int y):
	type(type),
	x(x),
	y(y)
{}

bool mouseEvent::IsValid() const
{
	return !type == EventType::INVALID;
}

mouseEvent::EventType mouseEvent::getType() const
{
	return type;
}

MousePoint mouseEvent::getPos() const
{
	return MousePoint({x,y});
}

int mouseEvent::getPosX() const
{
	return x;
}

int mouseEvent::getPosY() const
{
	return y;
}
