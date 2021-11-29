#include "Engine.h"
#include <Windows.h>

#define APP_HEIGHT 800
#define APP_WIDTH 600

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int cmdShow)
{
	Engine engine;
	engine.Initialise(hInstance, "App Title", "Window Class", APP_HEIGHT, APP_WIDTH, (GetSystemMetrics(SM_CXSCREEN) - APP_WIDTH) / 2, (GetSystemMetrics(SM_CYSCREEN) - APP_HEIGHT) / 2);

	while (engine.ProcessMessages())
	{
		engine.Update();
		engine.Render();
	}

	return EXIT_SUCCESS;
}