#pragma once
#include "MouseEvent.h"
#include <queue>

class Mouse
{
public:
	//Button Pressed Functions
	void LeftClickPressed(int, int);
	void RightClickPressed(int, int);
	void MiddlePressed(int, int);

	//Button Released Functions
	void LeftClickReleased(int, int);
	void RightClickReleased(int, int);
	void MiddleReleased(int, int);

	//Mouse Wheel / Movement Functions
	void WheelUp(int, int);
	void WheelDown(int, int);
	void MouseMove(int, int);
	void MouseRawMove(int, int);

	//Mouse Button Down Functions
	bool IsLeftClickDown();
	bool IsRightClickDown();
	bool IsMiddleDown();

	//Mouse Position Functions
	int GetXPos();
	int GetYPos();
	MousePos GetPos();

	bool IsEventBufferEmpty();
	MouseEvent GetEvent();

private:
	std::queue<MouseEvent> eventBuffer;
	bool leftClickDown = false;
	bool rightClickDown = false;
	bool middleDown = false;
	MousePos pos = {0,0};

};