#pragma once
#include <chrono>
#include <string>

///Timers used in this engine will use this structure
struct TimerStruct
{
	///Functions
	void StartTimer()
	{
		if (TimerEnabled)
			return;

		TimerEnabled = true;
		TimerStartPoint = std::chrono::high_resolution_clock::now();
	}

	void StopTimer()
	{
		if (!TimerEnabled)
			return;

		TimerEnabled = false;
		TimerEndPoint = std::chrono::high_resolution_clock::now();
	}

	void RestartFrameTimer()
	{
		StopTimer();
		StartTimer();
	}

	bool IsTimerActive()
	{
		return TimerEnabled;
	}

	float GetTimerMilliseconds()
	{
		if (TimerEnabled)
		{
			auto elapsedTime = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - TimerStartPoint);
			return elapsedTime.count();
		}

		auto elapsedTime = std::chrono::duration<float, std::milli>(TimerEndPoint - TimerStartPoint);
		return elapsedTime.count();
	}

	///Variables
	std::chrono::time_point<std::chrono::steady_clock> TimerStartPoint;
	std::chrono::time_point<std::chrono::steady_clock> TimerEndPoint;
	bool TimerEnabled;
};


class Timer
{
public:
	///Overall game timer / frame timer / float dt variable (frame timer)
	///Application Timer Functions
	static float GetApplicationLifetime();


	///Frame Timer (Used to monitor FPS Count, DEBUG)
	static TimerStruct fTimer;

	///Game Speed Timer (Allows movement to be consistent with FPS)
	static TimerStruct gTimer;


	static void IncrementFPSCounter();
	static void ResetFPSCounter();

	static void SetFPSString();
	static std::string& GetFPSString();

	static float dt;
private:
	///Application Lifetime timer
	static std::chrono::time_point<std::chrono::steady_clock> ApplicationTimerStartPoint;

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