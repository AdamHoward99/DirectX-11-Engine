#include "Engine.h"

bool Engine::Initialise(HINSTANCE hInstance, std::string title, std::string windowClass, int w, int h, int xCenter, int yCenter)
{
	if (!rw.InitialiseWindow(this, hInstance, title, windowClass, w, h, xCenter, yCenter))
		return false;

	if (!graphics.InitialiseClass(rw.GetWindowHandle(), w, h))
		return false;

	//Initialize Camera / Set Camera Properties
	camera.SetupCamera(DirectX::XMFLOAT3A(0.f, 0.f, -1.f), (float)w / float(h), 0.1f, 100.f);

	tr.Start();
	return true;
}

bool Engine::ProcessMessages()
{
	return this->rw.ProcessMessages();
}

void Engine::Update()
{
	float dt = tr.GetMilliseconds();
	tr.Reset();

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
		camera.MoveCameraForwardD(dt);

	if (keyboard.IsKeyPressed(CAMERA_BACK))
		camera.MoveCameraBackwardsD(dt);

	if (keyboard.IsKeyPressed(CAMERA_LEFT))
		camera.MoveCameraLeftD(dt);

	if (keyboard.IsKeyPressed(CAMERA_RIGHT))
		camera.MoveCameraRightD(dt);

	if (keyboard.IsKeyPressed(CAMERA_ELEVATE))
		camera.ElevateCameraD(dt);

	if (keyboard.IsKeyPressed(CAMERA_LOWER))
		camera.LowerCameraD(dt);
}

void Engine::Render()
{
	graphics.RenderFrame(&camera);
}