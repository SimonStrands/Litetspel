#pragma once
#include <Windows.h>
//git
bool getkey(char Key);

class Keyboard {
public:
	enum EventType
	{
		Press,
		Release,
		Invalid
	};
	Keyboard();
	bool isKeyPressed(unsigned char letter);
	//set keyReleased
	void onKeyReleased(const unsigned char letter);
	//set keyDown
	void onKeyDown(const unsigned char letter);

private:
	bool KeysDown[256];
	EventType type;

};
