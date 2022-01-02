#include "Timer.h"

///Static Variable Definitions
std::chrono::time_point<std::chrono::steady_clock> Timer::ApplicationTimerStartPoint = std::chrono::steady_clock::now();

TimerStruct Timer::fTimer;
TimerStruct Timer::gTimer;

int Timer::FPSCounter = 0;
std::string Timer::FPSString = "FPS: 0";
float Timer::dt = 0.0f;

float Timer::GetApplicationLifetime()
{
	///Get lifetime of this application at this current point
	auto elapsedTime = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - ApplicationTimerStartPoint);
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
