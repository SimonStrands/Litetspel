#pragma once
#include <SFML/Window.hpp>
#include "Keyboard.h"
#include "Vec.h"

class Mouse {
public:
	Mouse(vec2 WH);
	virtual ~Mouse();
	void changeSense(float newSence);
	void UpdateMouse();
	vec2 getDeltaPos();
	float getSense();
	bool getMouseActive();
private:
	void activateMouse();
	sf::Mouse mouse;
	bool mouse_active;
	bool once;
	vec2 wndWH;
	float x, y;
	float mouseSense;
};