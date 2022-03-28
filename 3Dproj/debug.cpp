#include "debug.h"
//git
#include <windows.h>

#include <stdio.h>

#include <fcntl.h>

#include <io.h>

#include <iostream>

#include <fstream>

#ifndef _USE_OLD_IOSTREAMS

using namespace std;

#endif

// maximum mumber of lines the output console should have

static const WORD MAX_CONSOLE_LINES = 500;

#ifdef _DEBUG

void RedirectIOToConsole()
{
    AllocConsole();
    HANDLE stdHandle;
    int hConsole;
    FILE* fp;
    stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    hConsole = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
    fp = _fdopen(hConsole, "w");

    freopen_s(&fp, "CONOUT$", "w", stdout);

    printf("Hello console on\n");
}
#endif

//End of File
