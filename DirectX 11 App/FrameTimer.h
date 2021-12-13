#pragma once
#include <chrono>
#include <string>

class FrameTimer
{
public:
	FrameTimer();

	void Start();
	void Stop();
	void Reset();
	bool IsActive() const { return active; }

	float GetMilliseconds();

	static void IncrementFPSCounter();
	static void ResetFPSCounter();

	static void SetFPSString();
	static std::string& GetFPSString();

private:
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
