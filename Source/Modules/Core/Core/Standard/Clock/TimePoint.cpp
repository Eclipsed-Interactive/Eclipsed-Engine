#include "Clock.h"

#include "../Core/Types.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
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