#pragma once

#include "Core.Core.hpp"
#include "TimePoint.h"

namespace Eclipse
{
	class CORE_API Clock final
	{
	public:
		static void Initialize();

		static TimePoint Now();
		static I64 Frequency();

	private:
		static I64 myFrequency;
	};
}