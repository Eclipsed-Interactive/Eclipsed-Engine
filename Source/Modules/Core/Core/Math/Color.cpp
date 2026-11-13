#include "Core/Math/Color.h"


namespace Eclipse::Math
{
	Color::Color(float R, float G, float B, float A)
		: r(R), g(G), b(B), a(A)
	{

	}

	Color::Color(int aHexValue)
	{
		r = ((aHexValue >> 16) & 0xFF) / oneOver255;
		g = ((aHexValue >> 8) & 0xFF) / oneOver255;
		b = ((aHexValue) & 0xFF) / oneOver255;
	}

	int32_t Color::GetHexValue()
	{
		uint32_t R = static_cast<uint32_t>(r * 255.f + 0.5f);  // round, not truncate
		uint32_t G = static_cast<uint32_t>(g * 255.f + 0.5f);
		uint32_t B = static_cast<uint32_t>(b * 255.f + 0.5f);
		uint32_t A = static_cast<uint32_t>(a * 255.f + 0.5f);
		return (R << 24) | (G << 16) | (B << 8) | A;
	}
}