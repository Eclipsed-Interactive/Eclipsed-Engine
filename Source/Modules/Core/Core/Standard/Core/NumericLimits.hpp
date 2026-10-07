#pragma once

#include "Types.hpp"

namespace Eclipse
{
	template<typename T>
	struct NumericLimits {};


	// =======================
	//      UNSIGNED INTEGERS
	// =======================
	template<>
	struct NumericLimits<U8>
	{
		static constexpr U8 max = ~U8{ 0 };
		static constexpr U8 min = 0;
	};

	template<>
	struct NumericLimits<U16>
	{
		static constexpr U16 max = ~U16{ 0 };
		static constexpr U16 min = 0;
	};

	template<>
	struct NumericLimits<U32>
	{
		static constexpr U32 max = ~U32{ 0 };
		static constexpr U32 min = 0;
	};

	template<>
	struct NumericLimits<U64>
	{
		static constexpr U64 max = ~U64{ 0 };
		static constexpr U64 min = 0;
	};


	// =======================
	//      SIGNED INTEGERS
	// =======================
	template<>
	struct NumericLimits<I8>
	{
		static constexpr I8 max = NumericLimits<U8>::max >> 1;
		static constexpr I8 min = -max - 1;
	};

	template<>
	struct NumericLimits<I16>
	{
		static constexpr I16 max = NumericLimits<U16>::max >> 1;
		static constexpr I16 min = -max - 1;
	};

	template<>
	struct NumericLimits<I32>
	{
		static constexpr I32 max = NumericLimits<U32>::max >> 1;
		static constexpr I32 min = -max - 1;
	};

	template<>
	struct NumericLimits<I64>
	{
		static constexpr I64 max = NumericLimits<U64>::max >> 1;
		static constexpr I64 min = -max - 1;
	};


	// =======================
	//      FLOATING POINTS
	// =======================
	template<>
	struct NumericLimits<F32>
	{
		static constexpr F32 min = 1.1754943508222875e-38F;
		static constexpr F32 lowest = -3.4028234663852886e+38F;
		static constexpr F32 max = 3.4028234663852886e+38F;
	};

	template<>
	struct NumericLimits<F64>
	{
		static constexpr F64 min = 2.2250738585072014e-308;
		static constexpr F64 lowest = -1.7976931348623157e+308;
		static constexpr F64 max = 1.7976931348623157e+308;
	};
}