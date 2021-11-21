#pragma once
#include "RenderWindow.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "DXGraphics.h"

class WindowContainer
{
public:
	WindowContainer();

	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	Keyboard keyboard;
	Mouse mouse;
	DXGraphics graphics;
protected:
	RenderWindow rw;


};