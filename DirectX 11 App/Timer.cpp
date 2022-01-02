#include "Timer.h"

std::chrono::time_point<std::chrono::steady_clock> Timer::ApplicationTimerStartPoint = std::chrono::high_resolution_clock::now();

int Timer::FPSCounter = 0;
std::string Timer::FPSString = "FPS: 0";

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

void Timer::Start()
{
	if (active)
		return;

	active = true;
	startPoint = std::chrono::high_resolution_clock::now();
}

void Timer::Stop()
{
	if (!active)
		return;

	active = false;
	EndPoint = std::chrono::high_resolution_clock::now();
}

void Timer::Reset()
{
	Stop();
	Start();
}

float Timer::GetMilliseconds()
{
	if(active)
	{
		auto elapsedTime = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - startPoint);
		return elapsedTime.count();	
	}

	auto elapsedTime = std::chrono::duration<float, std::milli>(EndPoint - startPoint);
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