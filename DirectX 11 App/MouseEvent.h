#pragma once

struct MousePos
{
	int x, y;
};

enum EventType
{
	LClick, LRelease, RClick, RRelease, MPress, MRelease, MWheelUp, MWheelDown, Move, RawMove, NullValue
};

class MouseEvent
{
public:
	MouseEvent();
	MouseEvent(const EventType, const int, const int);
	bool IsValid() const;
	int GetXPos() const;
	int GetYPos() const;
	EventType GetEventType() const;
	MousePos GetMousePosition() const;
private:
	EventType type;
	int x, y;
};