#pragma once
#include <chrono>

class FrameTimer
{
public:
	FrameTimer();
	float mark(); // Returns time in seconds from last call
private:
	std::chrono::steady_clock clk;
	std::chrono::steady_clock::time_point last_mark;
};

