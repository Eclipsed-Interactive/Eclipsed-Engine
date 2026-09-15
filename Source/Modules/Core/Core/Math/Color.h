#pragma once

#include "Core.Core.hpp"

#include "Vector/Vector4.h"

#include "cereal/cereal.hpp"

namespace Eclipse::Math
{
	constexpr float oneOver255 = 1.f / 255.f;

	class CORE_API Color final
	{
	public:
		Color() = default;
		
		Color(float R, float G, float B, float A);
		Color(int aHexValue);

		int GetHexValue();

		Vector4f ToVector() { return Vector4f(r, g, b, a); }

		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(CEREAL_NVP(r), CEREAL_NVP(g), CEREAL_NVP(b), CEREAL_NVP(a));
		}

		union
		{
			struct { float r, g, b, a; };
			float data[4] = { 1, 1, 1, 1 };
		};
	};
}