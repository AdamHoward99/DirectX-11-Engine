#pragma once

class KeyboardEvent
{
public:
	enum EventType
	{
		Press, Release, Invalid
	};

	KeyboardEvent();
	KeyboardEvent(const EventType, const unsigned char);
	~KeyboardEvent();

	bool IsPressed() const;
	bool IsReleased() const;
	bool IsValid() const;

	unsigned char GetKeyCode() const;

private:
	EventType type;
	unsigned char key;
};