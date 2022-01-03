#pragma once
#include <chrono>
#include <string>

class Timer
{
public:
	static float GetApplicationLifetime();

	void StartTimer();
	void StopTimer();
	void RestartFrameTimer();
	bool IsTimerActive();
	float GetTimerMilliseconds();

private:
	static std::chrono::time_point<std::chrono::steady_clock> ApplicationTimerStartPoint;

protected:

#ifdef _WIN32
	std::chrono::time_point<std::chrono::steady_clock> TimerStartPoint;
	std::chrono::time_point<std::chrono::steady_clock> TimerEndPoint;
#endif

#ifdef __unix__
	std::chrono::time_point<std::chrono::system_clock> startPoint;
	std::chrono::time_point<std::chrono::system_clock> EndPoint;
#endif

	bool TimerEnabled = false;
};
