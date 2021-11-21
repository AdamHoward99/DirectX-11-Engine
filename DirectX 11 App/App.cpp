#include "Engine.h"
#include <Windows.h>

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int cmdShow)
{
	Engine engine;
	engine.Initialise(hInstance, "App Title", "Window Class", 800, 600);

	while (engine.ProcessMessages())
	{
		engine.Update();
		engine.Render();
	}

	return EXIT_SUCCESS;
}