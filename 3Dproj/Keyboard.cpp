#include "Keyboard.h"

bool getkey(char Key)
{
	return (GetKeyState(Key) & 0x8000);
}

//Keyboard::Keyboard()
//{
//	for (int i = 0; i < 256; i++) {
//		KeysDown[i] = false;
//	}
//}
