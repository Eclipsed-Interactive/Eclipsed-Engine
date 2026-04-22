#pragma once

#include "Vector/Vector2.h"

#include "CoreEngine/Settings/SettingsBase.hpp"

namespace Eclipse::Math
{
	struct RectSizePos final
	{
		Math::Vector2f position;
		Math::Vector2f size;

		SERIALIZE(MAKE_NVP(position), MAKE_NVP(size))
	};
}