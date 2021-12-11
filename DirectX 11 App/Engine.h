#pragma once
#include "WindowCont.h"
#include "Camera.h"

#ifdef _DEBUG
#define CAMERA_FORWARD	87 //W
#define CAMERA_BACK		83 //S
#define CAMERA_LEFT		65 //A
#define CAMERA_RIGHT	68 //D
#define CAMERA_ELEVATE	81 //Q
#define CAMERA_LOWER	69 //E
#endif

class Engine : WindowContainer
{
public:
	bool Initialise(HINSTANCE, std::string, std::string, int, int, int, int);
	bool ProcessMessages();
	void Update();
	void Render();

private:
	Camera camera;
};