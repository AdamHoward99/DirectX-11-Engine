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

	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.LeftClickPressed(x, y);
		return EXIT_SUCCESS;
	}

	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.RightClickPressed(x, y);
		return EXIT_SUCCESS;
	}

	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.MiddlePressed(x, y);
		return EXIT_SUCCESS;
	}

	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.LeftClickReleased(x, y);
		return EXIT_SUCCESS;
	}

	case WM_RBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.RightClickReleased(x, y);
		return EXIT_SUCCESS;
	}

	case WM_MBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.MiddleReleased(x, y);
		return EXIT_SUCCESS;
	}

	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)		//Mouse wheel is moving up
			mouse.WheelUp(x, y);

		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)	//Mouse wheel is moving down
			mouse.WheelDown(x, y);

		return EXIT_SUCCESS;
	}




	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

}
