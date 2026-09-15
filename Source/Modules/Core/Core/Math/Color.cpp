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
		int R = static_cast<int>(r * 255.f);
		int G = static_cast<int>(g * 255.f);
		int B = static_cast<int>(b * 255.f);
		int A = static_cast<int>(a * 255.f);

		return (R << 24) | (G << 16) | (B << 8) | A;
	}
}