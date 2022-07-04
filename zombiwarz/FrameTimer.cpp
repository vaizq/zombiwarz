#include "FrameTimer.h"

FrameTimer::FrameTimer()
{
	last_mark = clk.now();
}

float FrameTimer::mark()
{
	auto last = last_mark;
	last_mark = clk.now();
	std::chrono::duration<float> diff = last_mark - last;
	return diff.count();
}

