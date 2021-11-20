#include "WindowCont.h"

LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	//Keyboard Messages
	case WM_KEYDOWN:
	{
		unsigned char c = static_cast<unsigned char>(wParam);
		if (keyboard.IsKeyAutoRepeating())
			keyboard.OnKeyPressed(c);
		else
		{
			const bool pressed = lParam & 0x40000000;
			if (!pressed)
				keyboard.OnKeyPressed(c);
		}
	}

	case WM_KEYUP:
	{
		unsigned char c = static_cast<unsigned char>(wParam);
		keyboard.OnKeyReleased(c);
		return EXIT_SUCCESS;
	}

	case WM_CHAR:
	{
		unsigned char c = static_cast<unsigned char>(wParam);
		if (keyboard.IsCharAutoRepeating())
			keyboard.OnChar(c);
		else
		{
			const bool pressed = lParam & 0x40000000;
			if (!pressed)
				keyboard.OnChar(c);
		}

		return EXIT_SUCCESS;
	}

	//Mouse Messages
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.MouseMove(x, y);

		return EXIT_SUCCESS;
	}

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);

	}

}
