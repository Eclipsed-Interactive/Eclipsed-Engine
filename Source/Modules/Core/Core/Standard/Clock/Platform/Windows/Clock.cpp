#include "Core/Standard/Clock/Clock.h"

#include "Core/Standard/Core/Types.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Eclipse
{
	I64 Clock::myFrequency;

	void Clock::Initialize()
	{
		//DebugBreak();

		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);

		myFrequency = frequency.QuadPart;
	}

	TimePoint Clock::Now()
	{
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);

		return TimePoint{ counter.QuadPart };
	}
	
	I64 Clock::Frequency()
	{
		return myFrequency;
	}
}