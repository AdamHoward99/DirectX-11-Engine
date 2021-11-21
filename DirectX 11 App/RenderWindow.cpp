#include "WindowCont.h"

RenderWindow::RenderWindow()
{}

RenderWindow::~RenderWindow()
{
	//Delete Objects
	if (this->mWindowInfo.handle != NULL)
	{
		UnregisterClass(this->mWindowInfo.applicationClass.c_str(), this->mWindowInfo.hInstance);
		DestroyWindow(this->mWindowInfo.handle);
	}
}

bool RenderWindow::InitialiseWindow(WindowContainer* pWindowCont,HINSTANCE hInstance, std::string appTitle, std::string appClass, int width, int height)
{
	this->mWindowInfo.hInstance = hInstance;
	this->mWindowInfo.applicationTitle = appTitle;
	this->mWindowInfo.applicationClass = appClass;
	this->mWindowInfo.height = height;
	this->mWindowInfo.width = width;

	RegisterWindowClass();

	//Create Window
	this->mWindowInfo.handle = CreateWindowEx(NULL,
		this->mWindowInfo.applicationClass.c_str(),
		this->mWindowInfo.applicationTitle.c_str(),
		WS_MINIMIZEBOX | WS_CAPTION | WS_SYSMENU,
		0, 0,
		this->mWindowInfo.width, this->mWindowInfo.height,
		NULL,
		NULL,
		this->mWindowInfo.hInstance,
		pWindowCont);

	//Ensure Window was created
	if (this->mWindowInfo.handle == NULL)
	{
		MessageBox(this->mWindowInfo.handle, "CreateWindowEx function failed. Closing Application", "Error", MB_OK);
		return false;
	}

	//Focus on window
	ShowWindow(this->mWindowInfo.handle, SW_SHOW);
	SetForegroundWindow(this->mWindowInfo.handle);
	SetFocus(this->mWindowInfo.handle);

	return true;
}

LRESULT CALLBACK HandleMessageRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return NULL;

	default:
	{
		WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
}

LRESULT CALLBACK HandleMessageStart(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NCCREATE:
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
		if (pWindow == nullptr)
			exit(-1);

		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMessageRedirect));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}


void RenderWindow::RegisterWindowClass()
{
	WNDCLASSEX wClass;
	wClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wClass.hInstance = this->mWindowInfo.hInstance;
	wClass.hIcon = NULL;
	wClass.hIconSm = NULL;
	wClass.lpfnWndProc = HandleMessageStart;
	wClass.cbClsExtra = NULL;
	wClass.cbWndExtra = NULL;
	wClass.hbrBackground = NULL;
	wClass.lpszMenuName = NULL;
	wClass.lpszClassName = this->mWindowInfo.applicationClass.c_str();
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wClass);
}

bool RenderWindow::ProcessMessages()
{
	//Handle Incoming Messages
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (PeekMessage(&msg, this->mWindowInfo.handle, NULL, NULL, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT)
	{
		if (!IsWindow(this->mWindowInfo.handle))
		{
			this->mWindowInfo.handle = NULL;
			UnregisterClass(this->mWindowInfo.applicationClass.c_str(), this->mWindowInfo.hInstance);
			return false;
		}
	}

	return true;
}

