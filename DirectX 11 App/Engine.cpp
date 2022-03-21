#include "Engine.h"

bool Engine::Initialise(HINSTANCE hInstance, std::string title, std::string windowClass, int w, int h, int xCenter, int yCenter)
{
	if (!rw.InitialiseWindow(this, hInstance, title, windowClass, w, h, xCenter, yCenter))
		return false;

	if (!graphics.InitialiseClass(rw.GetWindowHandle(), w, h))
		return false;

	//Initialize Camera / Set Camera Properties
	camera.SetupCamera(DirectX::XMFLOAT3A(0.f, 0.f, -3.f), (float)w / float(h), 0.1f, 100.f);

	gTimer.StartTimer();
	return true;
}

bool Engine::ProcessMessages()
{
	return this->rw.ProcessMessages();
}

void Engine::Update()
{
	gTimer.UpdateDeltaTime();
	gTimer.RestartFrameTimer();

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
		if (mouseEv.GetEventType() == EventType::RawMove && camera.GetMouseMovement())
			camera.AddToRotation(DirectX::XMFLOAT3A(mouseEv.GetYPos() * 0.001f, mouseEv.GetXPos() * 0.001f, 0.f));
	}

	//Mouse Camera Controls
	if (keyboard.IsKeyPressed(CAMERA_MOUSE_ENABLE))
		camera.SetMouseMovement(true);

	if (keyboard.IsKeyPressed(CAMERA_MOUSE_DISABLE))
		camera.SetMouseMovement(false);

	if (keyboard.IsKeyPressed(CAMERA_FORWARD))
		camera.MoveCameraForward3A_D(gTimer.GetDeltaTime());

	if (keyboard.IsKeyPressed(CAMERA_BACK))
		camera.MoveCameraBackwards3A_D(gTimer.GetDeltaTime());

	if (keyboard.IsKeyPressed(CAMERA_LEFT))
		camera.MoveCameraLeft3A_D(gTimer.GetDeltaTime());

	if (keyboard.IsKeyPressed(CAMERA_RIGHT))
		camera.MoveCameraRight3A_D(gTimer.GetDeltaTime());

	if (keyboard.IsKeyPressed(CAMERA_ELEVATE))
		camera.ElevateCamera3A_D(gTimer.GetDeltaTime());

	if (keyboard.IsKeyPressed(CAMERA_LOWER))
		camera.LowerCamera3A_D(gTimer.GetDeltaTime());
}

void Engine::Render()
{
	graphics.RenderFrame(&camera, gTimer.GetDeltaTime());
}