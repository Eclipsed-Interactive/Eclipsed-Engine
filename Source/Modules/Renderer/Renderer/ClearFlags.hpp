#pragma once

namespace Eclipse::Graphics
{
	enum class ClearFlags : unsigned char
	{
		None = 0,
		Color = 1 << 0,
		Depth = 1 << 1,
		Stencil = 1 << 2
	};

	inline ClearFlags operator|(ClearFlags a, ClearFlags b)
	{
		return static_cast<ClearFlags>(
			static_cast<unsigned char>(a) |
			static_cast<unsigned char>(b)
			);
	}

	inline ClearFlags operator&(ClearFlags a, ClearFlags b)
	{
		return static_cast<ClearFlags>(
			static_cast<unsigned char>(a) &
			static_cast<unsigned char>(b)
			);
	}
}