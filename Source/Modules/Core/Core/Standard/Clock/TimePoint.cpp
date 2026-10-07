#include "Clock.h"

#include "../Core/Types.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Eclipse
{
	constexpr TimePoint::TimePoint(I64 ticks)
		: myTicks(ticks)
	{
	}

	constexpr Duration TimePoint::operator-(const TimePoint& other) const
	{
		return Duration::FromTicks(myTicks - other.myTicks);
	}
}