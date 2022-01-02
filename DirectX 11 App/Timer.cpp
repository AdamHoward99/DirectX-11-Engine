#include "Timer.h"

Timer::Timer()
{
	startPoint = std::chrono::high_resolution_clock::now();
}

float Timer::GetApplicationLifetime()
{
	///Get lifetime of this application at this current point
	auto elapsedTime = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - ApplicationTimerStartPoint);
	return elapsedTime.count();
}

void Timer::StartFrameTimer()
{
	if (FrameTimerEnabled)
		return;

	FrameTimerEnabled = true;
	FrameTimerStartPoint = std::chrono::high_resolution_clock::now();
}

void Timer::StopFrameTimer()
{
	if (!FrameTimerEnabled)
		return;

	FrameTimerEnabled = false;
	FrameTimerEndPoint = std::chrono::high_resolution_clock::now();
}

void Timer::RestartFrameTimer()
{
	StopFrameTimer();
	StartFrameTimer();
}

bool Timer::IsFrameTimerActive()
{
	return FrameTimerEnabled;
}

float Timer::GetFrameTimerMilliseconds()
{
	if (FrameTimerEnabled)
	{
		auto elapsedTime = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - FrameTimerStartPoint);
		return elapsedTime.count();
	}

	auto elapsedTime = std::chrono::duration<float, std::milli>(FrameTimerEndPoint - FrameTimerStartPoint);
	return elapsedTime.count();
}

void Timer::IncrementFPSCounter()
{
	FPSCounter++;
}

void Timer::ResetFPSCounter()
{
	FPSCounter = 0;
}

void Timer::SetFPSString()
{
	FPSString = "FPS: " + std::to_string(FPSCounter);
}

std::string& Timer::GetFPSString()
{
	return FPSString;
}