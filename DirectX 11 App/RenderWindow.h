#pragma once
#include <string>
#include <Windows.h>

class WindowContainer;

struct WindowInfo
{
	HWND handle = NULL;
	HINSTANCE hInstance = NULL;
	std::string applicationTitle = "";
	std::string applicationClass = "";
	int height = 0;
	int width = 0;
	RECT windowRect;
};

class RenderWindow
{
public:
	RenderWindow();
	~RenderWindow();

	bool InitialiseWindow(WindowContainer* pWindowCont, HINSTANCE, std::string, std::string, int, int, int, int);
	bool ProcessMessages();
	HWND GetWindowHandle() const { return mWindowInfo.handle; }
private:
	void RegisterWindowClass();
	WindowInfo mWindowInfo;
};
