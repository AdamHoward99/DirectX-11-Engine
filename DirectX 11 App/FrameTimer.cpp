#include "FrameTimer.h"

int FrameTimer::FPSCounter = 0;
std::string FrameTimer::FPSString = "FPS: 0";

FrameTimer::FrameTimer()
{
	startPoint = std::chrono::high_resolution_clock::now();
}

void FrameTimer::Start()
{
	if (active)
		return;

	active = true;
	startPoint = std::chrono::high_resolution_clock::now();
}

void FrameTimer::Stop()
{
	if (!active)
		return;

	active = false;
	EndPoint = std::chrono::high_resolution_clock::now();
}

void FrameTimer::Reset()
{
	Stop();
	Start();
}

double FrameTimer::GetMilliseconds()
{
	auto elapsedTime = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - startPoint);

	//auto elapsedTime = std::chrono::duration<float>(EndPoint - startPoint);
	return elapsedTime.count();
}

void FrameTimer::IncrementFPSCounter()
{
	FPSCounter++;
}

void FrameTimer::ResetFPSCounter()
{
	FPSCounter = 0;
}

void FrameTimer::SetFPSString()
{
	FPSString = "FPS: " + std::to_string(FPSCounter);
}

std::string& FrameTimer::GetFPSString()
{
	return FPSString;
}