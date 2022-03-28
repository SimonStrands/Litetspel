#include "Mouse.h"
#include <iostream>

//use sfml
Mouse::Mouse(vec2 WH):
	wndWH(WH.x, WH.y)
{
	mouseSense = 0.6;
	mouse_active = false;
	once = false;
	x = y = 0;
}

Mouse::~Mouse()
{
}

void Mouse::changeSense(float newSence)
{
	this->mouseSense = newSence;
}
int x = 0;
void Mouse::UpdateMouse()
{
	
	activateMouse();
	if (mouse_active) {
		::ShowCursor(FALSE);
		x = (mouse.getPosition().x - this->wndWH.x/2) * mouseSense;
		y = (mouse.getPosition().y - this->wndWH.y/2) * mouseSense;
		mouse.setPosition(sf::Vector2i((int)this->wndWH.x/2, (int)this->wndWH.y/2));
	}
	else {
		::ShowCursor(TRUE);
	}
}

vec2 Mouse::getDeltaPos()
{
	return vec2(x,y);
}

float Mouse::getSense()
{
	return mouseSense;
}

bool Mouse::getMouseActive()
{
	return mouse_active;
}

void Mouse::activateMouse()
{
	if (GetKeyState(VK_ESCAPE) & 0x8000 || GetKeyState(VK_TAB) & 0x8000) {
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
}
