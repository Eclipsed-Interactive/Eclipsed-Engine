#include "Clock.h"

#include "../Core/Types.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Eclipse
{
	constexpr Duration::Duration(I64 ticks)
		: myTicks(ticks)
	{
	}

	constexpr Duration Duration::FromTicks(I64 ticks)
	{
		return Duration(ticks);
	}

	I64 Duration::ToNanoSeconds() const { return (myTicks * 1'000'000'000LL) / Clock::Frequency(); };

	I64 Duration::ToMicroSeconds() const { return ToNanoSeconds() * 0.001f; };
	F64 Duration::ToMilliSeconds() const { return ToNanoSeconds() * 0.000001f; };
	F64 Duration::ToSeconds() const { return ToNanoSeconds() * 0.000000001f; };

	constexpr Duration Duration::operator*(F64 scale) const
	{
		return Duration::FromTicks(static_cast<I64>(static_cast<F64>(myTicks) * scale));
	}

	constexpr Duration Duration::operator+=(Duration other)
	{
		return Duration::FromTicks(this->myTicks + other.myTicks);
	}
}