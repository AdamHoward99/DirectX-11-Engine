#include "Engine.h"

bool Engine::Initialise(HINSTANCE hInstance, std::string title, std::string windowClass, int w, int h, int xCenter, int yCenter)
{
	if (!rw.InitialiseWindow(this, hInstance, title, windowClass, w, h, xCenter, yCenter))
		return false;

	if (!graphics.InitialiseClass(rw.GetWindowHandle(), w, h))
		return false;

	return true;
}

bool Engine::ProcessMessages()
{
	return this->rw.ProcessMessages();
}

void Engine::Update()
{
	while (!keyboard.IsCharBufferEmpty())
	{
		unsigned char c = keyboard.ReadChar();
	}

	while (!keyboard.IsKeyBufferEmpty())
	{
		unsigned char keycode = keyboard.ReadKey().GetKeyCode();
	}

	while (!mouse.IsEventBufferEmpty())
	{
		MouseEvent mouseEv = mouse.GetEvent();
	}
}

void Engine::Render()
{
	graphics.RenderFrame();
}