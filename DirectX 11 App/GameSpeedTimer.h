#pragma once
#include "Timer.h"

///Inherits some functionality from Timer base class, used to monitor speed of in-game objects with current framerate
class GameSpeedTimer : public Timer
{
public:
	const void UpdateDeltaTime()
	{
		deltaTime = GetTimerMilliseconds();
	}

	const float GetDeltaTime() const
	{
		return deltaTime;
	}

private:
	float deltaTime = 0.0f;
};