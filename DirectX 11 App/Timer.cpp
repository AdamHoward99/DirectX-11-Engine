#include "Timer.h"

///Static Variable Definition
std::chrono::time_point<std::chrono::steady_clock> Timer::ApplicationTimerStartPoint = std::chrono::high_resolution_clock::now();

const float Timer::GetApplicationLifetime()
{
	///Get lifetime of this application at this current point
	auto elapsedTime = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - ApplicationTimerStartPoint);
	return elapsedTime.count();
}

const void Timer::StartTimer()
{
	if (TimerEnabled)
		return;

	TimerEnabled = true;
	TimerStartPoint = std::chrono::high_resolution_clock::now();
}

const void Timer::StopTimer()
{
	if (!TimerEnabled)
		return;

	TimerEnabled = false;
	TimerEndPoint = std::chrono::high_resolution_clock::now();
}

const void Timer::RestartFrameTimer()
{
	StopTimer();
	StartTimer();
}

const bool Timer::IsTimerActive() const
{
	return TimerEnabled;
}

const float Timer::GetTimerMilliseconds() const
{
	if (TimerEnabled)
	{
		auto elapsedTime = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - TimerStartPoint);
		return elapsedTime.count();
	}

	auto elapsedTime = std::chrono::duration<float, std::milli>(TimerEndPoint - TimerStartPoint);
	return elapsedTime.count();
}