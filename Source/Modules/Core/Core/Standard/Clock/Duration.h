#pragma once

#include "Core.Core.hpp"
#include "../Core/Types.hpp"

namespace Eclipse
{
	class CORE_API Duration final
	{
	public:
		Duration() = default;
		constexpr Duration(I64 ticks);

	public:
		static constexpr Duration FromTicks(I64 ticks);

	public:
		I64 ToNanoSeconds() const;

		I64 ToMicroSeconds() const;
		F64 ToMilliSeconds() const;
		F64 ToSeconds() const;

	public:
		constexpr Duration operator*(F64 scale) const;
		constexpr Duration operator+=(Duration other);

	private:
		I64 myTicks = 0;
	};
}