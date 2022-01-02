#pragma once
#include <chrono>
#include <string>

class Timer
{
public:
	Timer();

	///Overall game timer / frame timer / float dt variable (frame timer)
	///Application Timer Functions
	static float GetApplicationLifetime();

	///Frame Timer Functions
	static void StartFrameTimer();
	static void StopFrameTimer();
	static void RestartFrameTimer();
	static bool IsFrameTimerActive();

	static float GetFrameTimerMilliseconds();

	static void IncrementFPSCounter();
	static void ResetFPSCounter();

	static void SetFPSString();
	static std::string& GetFPSString();

private:
	///Application Lifetime timer
	static std::chrono::time_point<std::chrono::steady_clock> ApplicationTimerStartPoint;

	///Frame timer
	static std::chrono::time_point<std::chrono::steady_clock> FrameTimerStartPoint;
	static std::chrono::time_point<std::chrono::steady_clock> FrameTimerEndPoint;
	static bool FrameTimerEnabled;

	bool active = false;
	static int FPSCounter;
	static std::string FPSString;

#ifdef _WIN32
	std::chrono::time_point<std::chrono::steady_clock> startPoint;
	std::chrono::time_point<std::chrono::steady_clock> EndPoint;
#endif

#ifdef __unix__
	std::chrono::time_point<std::chrono::system_clock> startPoint;
	std::chrono::time_point<std::chrono::system_clock> EndPoint;
#endif
};

///Static Variable Definitions
std::chrono::time_point<std::chrono::steady_clock> Timer::ApplicationTimerStartPoint = std::chrono::high_resolution_clock::now();

static std::chrono::time_point<std::chrono::steady_clock> FrameTimerStartPoint = std::chrono::high_resolution_clock::now();
static std::chrono::time_point<std::chrono::steady_clock> FrameTimerEndPoint = std::chrono::high_resolution_clock::now();
static bool FrameTimerEnabled = false;

int Timer::FPSCounter = 0;
std::string Timer::FPSString = "FPS: 0";