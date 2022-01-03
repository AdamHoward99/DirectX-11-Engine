#pragma once
#include "Timer.h"

///Inherits some functionality from Timer base class, used to monitor framerate
class FrameTimer : public Timer
{
public:
	const void IncrementFPSCounter()
	{
		FPSCounter++;
	};

	const void ResetFPSCounter()
	{
		FPSCounter = 0;
	}

	const void SetFPSString()
	{
		FPSString = "FPS: " + std::to_string(FPSCounter);
	}

	const std::string& GetFPSString() const
	{
		return FPSString;
	}

private:
	int FPSCounter = 0;
	std::string FPSString = "FPS: 0";
};