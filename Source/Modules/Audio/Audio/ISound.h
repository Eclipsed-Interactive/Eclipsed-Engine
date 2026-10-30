#pragma once

#include <Core/Math/Vector/Vector2.h>

namespace Eclipse::Audio
{
	class ISound
	{
	public:
		virtual ~ISound() = default;

		virtual void* GetSound() = 0;
		virtual void SetSound(void* aSound) = 0;
	};
}