#pragma once

#include <Windows.h>


class Console
{
public:
    Console();
    ~Console();
private:
    DWORD previousMode;
    HANDLE consoleHandle;
};
