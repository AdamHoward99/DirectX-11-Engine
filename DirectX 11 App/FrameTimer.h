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
	std::chrono::time_point<std::chrono::steady_clock> startPoint;
	std::chrono::time_point<std::chrono::steady_clock> EndPoint;
	//TODO: include system_clock versions for LINUX OS

	bool active = false;
	static int FPSCounter;
	static std::string FPSString;
};
