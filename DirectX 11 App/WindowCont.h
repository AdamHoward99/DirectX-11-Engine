#pragma once
#include "RenderWindow.h"
#include "Keyboard.h"
#include "Mouse.h"

class WindowContainer
{
public:
	WindowContainer();

	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	Keyboard keyboard;
	Mouse mouse;
protected:
	RenderWindow rw;


};