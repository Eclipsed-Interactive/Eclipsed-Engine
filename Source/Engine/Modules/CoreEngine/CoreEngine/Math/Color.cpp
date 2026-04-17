#include "pch.h"

#include "Color.h"


namespace Eclipse
{
	Math::Color::Color(float R, float G, float B, float A)
		: r(R), g(G), b(B), a(A)
	{

	}

	Math::Color::Color(int aHexValue)
	{
		r = ((aHexValue >> 16) & 0xFF) / 255.f;
		g = ((aHexValue >> 8) & 0xFF) / 255.f;
		b = ((aHexValue) & 0xFF) / 255.f;
	}
}