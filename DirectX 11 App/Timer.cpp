#include "Timer.h"

///Static Variable Definitions
std::chrono::time_point<std::chrono::steady_clock> Timer::ApplicationTimerStartPoint = std::chrono::high_resolution_clock::now();

float Timer::GetApplicationLifetime()
{
	///Get lifetime of this application at this current point
	auto elapsedTime = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - ApplicationTimerStartPoint);
	return elapsedTime.count();
}

void Timer::StartTimer()
{
	if (TimerEnabled)
		return;

	TimerEnabled = true;
	TimerStartPoint = std::chrono::high_resolution_clock::now();
}

void Timer::StopTimer()
{
	if (!TimerEnabled)
		return;

	TimerEnabled = false;
	TimerEndPoint = std::chrono::high_resolution_clock::now();
}

void Timer::RestartFrameTimer()
{
	StopTimer();
	StartTimer();
}

bool Timer::IsTimerActive()
{
	return TimerEnabled;
}

float Timer::GetTimerMilliseconds()
{
	if (TimerEnabled)
	{
		auto elapsedTime = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - TimerStartPoint);
		return elapsedTime.count();
	}

	auto elapsedTime = std::chrono::duration<float, std::milli>(TimerEndPoint - TimerStartPoint);
	return elapsedTime.count();
}