#pragma once
#include "WindowCont.h"

class Engine : WindowContainer
{
public:
	bool Initialise(HINSTANCE, std::string, std::string, int, int);
	bool ProcessMessages();
	void Update();
	void Render();



private:
};