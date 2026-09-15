#pragma once

#include "Vector/Vector2.h"
#include "cereal/cereal.hpp"


namespace Eclipse::Math
{
	struct RectSizePos final
	{
		Math::Vector2f position;
		Math::Vector2f size;

		template <class Archive>
		void serialize(Archive& ar, const std::uint32_t version)
		{
			ar(CEREAL_NVP(position), CEREAL_NVP(size));
		}
	};
}