#include "Engine.h"

bool Engine::Initialise(HINSTANCE hInstance, std::string title, std::string windowClass, int w, int h, int xCenter, int yCenter)
{
	if (!rw.InitialiseWindow(this, hInstance, title, windowClass, w, h, xCenter, yCenter))
		return false;

	if (!graphics.InitialiseClass(rw.GetWindowHandle(), w, h))
		return false;

	//Initialize Camera / Set Camera Properties
	camera.SetupCamera(DirectX::XMFLOAT3A(0.f, 0.f, -1.f), (float)w / float(h), 0.1f, 100.f);
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

	//Mouse Camera Controls
	if (keyboard.IsKeyPressed(CAMERA_FORWARD))
		camera.MoveCameraForwardD();

	if (keyboard.IsKeyPressed(CAMERA_BACK))
		camera.MoveCameraBackwardsD();

	if (keyboard.IsKeyPressed(CAMERA_LEFT))
		camera.MoveCameraLeftD();

	if (keyboard.IsKeyPressed(CAMERA_RIGHT))
		camera.MoveCameraRightD();
}

void Engine::Render()
{
	graphics.RenderFrame(&camera);
}