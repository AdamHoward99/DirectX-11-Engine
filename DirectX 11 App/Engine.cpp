#include "Engine.h"

bool Engine::Initialise(HINSTANCE hInstance, std::string title, std::string windowClass, int w, int h, int xCenter, int yCenter)
{
	if (!rw.InitialiseWindow(this, hInstance, title, windowClass, w, h, xCenter, yCenter))
		return false;

	if (!graphics.InitialiseClass(rw.GetWindowHandle(), w, h))
		return false;

	//Initialize Camera / Set Camera Properties
	camera.SetupCamera(DirectX::XMFLOAT3A(0.f, 0.f, -1.f), (float)w / float(h), 0.1f, 100.f);

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
			camera.MoveRotation(DirectX::XMFLOAT3A(mouseEv.GetYPos() * 0.001f, mouseEv.GetXPos() * 0.001f, 0.f));
	}

	//Mouse Camera Controls
	if (keyboard.IsKeyPressed(CAMERA_MOUSE_ENABLE))
		camera.SetMouseMovement(true);

	if (keyboard.IsKeyPressed(CAMERA_MOUSE_DISABLE))
		camera.SetMouseMovement(false);

	if (keyboard.IsKeyPressed(CAMERA_FORWARD))
		camera.MoveCameraForwardD(gTimer.GetDeltaTime());

	if (keyboard.IsKeyPressed(CAMERA_BACK))
		camera.MoveCameraBackwardsD(gTimer.GetDeltaTime());

	if (keyboard.IsKeyPressed(CAMERA_LEFT))
		camera.MoveCameraLeftD(gTimer.GetDeltaTime());

	if (keyboard.IsKeyPressed(CAMERA_RIGHT))
		camera.MoveCameraRightD(gTimer.GetDeltaTime());

	if (keyboard.IsKeyPressed(CAMERA_ELEVATE))
		camera.ElevateCameraD(gTimer.GetDeltaTime());

	if (keyboard.IsKeyPressed(CAMERA_LOWER))
		camera.LowerCameraD(gTimer.GetDeltaTime());
}

void Engine::Render()
{
	graphics.RenderFrame(&camera);
}