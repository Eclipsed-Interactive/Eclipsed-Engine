#pragma once

#include "Core.Core.hpp"
#include "../Core/Types.hpp"
#include "Duration.h"

namespace Eclipse
{
	class CORE_API TimePoint final
	{
	public:
		TimePoint() = default;
		constexpr TimePoint(I64 ticks);

	public:
		constexpr Duration operator-(const TimePoint& other) const;

	private:
		I64 myTicks;
	};
}