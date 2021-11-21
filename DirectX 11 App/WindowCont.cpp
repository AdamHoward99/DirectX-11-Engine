#include "WindowCont.h"

WindowContainer::WindowContainer()
{
	static bool isRawInputInitialized = false;
	if (!isRawInputInitialized)
	{
		RAWINPUTDEVICE rawDevice;
		rawDevice.dwFlags = 0;
		rawDevice.hwndTarget = NULL;
		rawDevice.usUsage = 0x02;
		rawDevice.usUsagePage = 0x01;		//Correlates to Mouse

		if (!RegisterRawInputDevices(&rawDevice, 1, sizeof rawDevice))
		{
			OutputDebugStringA("Input Device failed!");
			isRawInputInitialized = true;
			exit(-1);
		}
	}
}

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

	case WM_INPUT:
	{
		UINT rawDataSize;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &rawDataSize, sizeof RAWINPUTHEADER);

		if (rawDataSize > 0)
		{
			std::unique_ptr<BYTE[]> data = std::make_unique<BYTE[]>(rawDataSize);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, data.get(), &rawDataSize, sizeof RAWINPUTHEADER) == rawDataSize)
			{
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(data.get());
				if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					mouse.MouseRawMove(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				}
			}
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}




	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

}
