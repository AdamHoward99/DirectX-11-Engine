#pragma once
#include <chrono>
#include <string>

class Timer
{
public:
	///Returns total time since application was launched
	static const float GetApplicationLifetime();

	///Functionality which is inherited by GameSpeedTimer & FrameTimer classes
	const void StartTimer();
	const void StopTimer();
	const void RestartFrameTimer();
	const bool IsTimerActive() const;
	const float GetTimerMilliseconds() const;

private:
	static std::chrono::time_point<std::chrono::steady_clock> ApplicationTimerStartPoint;
	bool TimerEnabled = false;

#ifdef _WIN32
	std::chrono::time_point<std::chrono::steady_clock> TimerStartPoint;
	std::chrono::time_point<std::chrono::steady_clock> TimerEndPoint;
#endif

#ifdef __unix__
	std::chrono::time_point<std::chrono::system_clock> startPoint;
	std::chrono::time_point<std::chrono::system_clock> EndPoint;
#endif
};
