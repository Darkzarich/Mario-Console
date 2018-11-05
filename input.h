#pragma once

#include <windows.h>


bool IsKeyDown(int virtualKeyCode)
{
    short keyState = GetAsyncKeyState(virtualKeyCode);
    return ((keyState & 0x8000) > 0);
}
