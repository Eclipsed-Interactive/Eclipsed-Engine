#pragma once

#include "CoreEngine/Math/Vector/Vector4.h"
#include "CoreEngine/Settings/SettingsBase.hpp"

namespace Eclipse::Math
{
	class Color final
	{
	public:
		Color(float R = 1.f, float G = 1.f, float B = 1.f, float A = 1.f);
		Color(int aHexValue);

		int32_t GetHexValue() 
		{
			int R = static_cast<int>(r * 255.f);
			int G = static_cast<int>(g * 255.f);
			int B = static_cast<int>(b * 255.f);
			int A = static_cast<int>(a * 255.f);

			return (R << 24) | (G << 16) | (B << 8) | A;
		}

		Vector4f ToVector() { return { r, g, b, a }; }

		SERIALIZE(MAKE_NVP(r),
			MAKE_NVP(g),
			MAKE_NVP(b),
			MAKE_NVP(a))

	public:
		union
		{
			struct { float r, g, b, a; };
			float data[4] = { 1, 1, 1, 1 };
		};
	};
}