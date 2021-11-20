#pragma once
#include "RenderWindow.h"
#include "Keyboard.h"

class WindowContainer
{
public:
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	Keyboard keyboard;
protected:
	RenderWindow rw;


};