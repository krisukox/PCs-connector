#include "Console.hpp"

Console::Console()
{
    consoleHandle = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(consoleHandle, &previousMode);
    SetConsoleMode(consoleHandle, previousMode & ENABLE_EXTENDED_FLAGS);
}

Console::~Console()
{
    SetConsoleMode(consoleHandle, previousMode);
}
