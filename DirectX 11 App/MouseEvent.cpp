#include "MouseEvent.h"

MouseEvent::MouseEvent()
	: type(EventType::NullValue), x(0), y(0)
{}

MouseEvent::MouseEvent(const EventType type, const int x, const int y)
	:type(type), x(x), y(y)
{}

bool MouseEvent::IsValid() const
{
	return type != EventType::NullValue;
}

int MouseEvent::GetXPos() const
{
	return x;
}

int MouseEvent::GetYPos() const
{
	return y;
}

EventType MouseEvent::GetEventType() const
{
	return type;
}

MousePos MouseEvent::GetMousePosition() const
{
	return {x, y};
}
