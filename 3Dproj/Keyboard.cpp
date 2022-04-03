#include "Keyboard.h"

bool getkey(char Key)
{
	return (GetKeyState(Key) & 0x8000);
}

Keyboard::Keyboard()
{
	for (int i = 0; i < 256; i++) {
		KeysDown[i] = false;
	}
	type = EventType::Invalid;
}

bool Keyboard::isKeyPressed(unsigned char letter)
{
	return KeysDown[letter];
}

void Keyboard::onKeyReleased(const unsigned char letter)
{
	KeysDown[letter] = false;
}

void Keyboard::onKeyDown(const unsigned char letter)
{
	KeysDown[letter] = true;
}
