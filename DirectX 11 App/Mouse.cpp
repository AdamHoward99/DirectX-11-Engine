#include "Mouse.h"

void Mouse::LeftClickPressed(int x, int y)
{
	leftClickDown = true;
	eventBuffer.push(MouseEvent(EventType::LClick, x, y));
}

void Mouse::RightClickPressed(int x, int y)
{
	rightClickDown = true;
	eventBuffer.push(MouseEvent(EventType::RClick, x, y));
}

void Mouse::MiddlePressed(int x, int y)
{
	middleDown = true;
	eventBuffer.push(MouseEvent(EventType::MPress, x, y));
}

void Mouse::LeftClickReleased(int x, int y)
{
	leftClickDown = false;
	eventBuffer.push(MouseEvent(EventType::LRelease, x, y));
}

void Mouse::RightClickReleased(int x, int y)
{
	rightClickDown = false;
	eventBuffer.push(MouseEvent(EventType::RRelease, x, y));
}

void Mouse::MiddleReleased(int x, int y)
{
	middleDown = false;
	eventBuffer.push(MouseEvent(EventType::MRelease, x, y));
}

void Mouse::WheelUp(int x, int y)
{
	eventBuffer.push(MouseEvent(EventType::MWheelUp, x, y));
}

void Mouse::WheelDown(int x, int y)
{
	eventBuffer.push(MouseEvent(EventType::MWheelDown, x, y));
}

void Mouse::MouseMove(int x, int y)
{
	x = x;
	y = y;
	eventBuffer.push(MouseEvent(EventType::Move, x, y));
}

void Mouse::MouseRawMove(int x, int y)
{
	eventBuffer.push(MouseEvent(EventType::RawMove, x, y));
}

bool Mouse::IsLeftClickDown()
{
	return leftClickDown;
}

bool Mouse::IsRightClickDown()
{
	return rightClickDown;
}

bool Mouse::IsMiddleDown()
{
	return middleDown;
}

int Mouse::GetXPos()
{
	return pos.x;
}

int Mouse::GetYPos()
{
	return pos.y;
}

MousePos Mouse::GetPos()
{
	return pos;
}

bool Mouse::IsEventBufferEmpty()
{
	return eventBuffer.empty();
}

MouseEvent Mouse::GetEvent()
{
	if (eventBuffer.empty())
		return MouseEvent();
	
	MouseEvent mouseEv = eventBuffer.front();
	eventBuffer.pop();
	return mouseEv;
}
