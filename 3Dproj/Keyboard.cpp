#include "Keyboard.h"
//git
bool getkey(char Key)
{
	return (GetKeyState(Key) & 0x8000);
}
