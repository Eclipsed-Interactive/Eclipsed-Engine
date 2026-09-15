#include "AbstractInput.h"

namespace Eclipse::Input
{
	void AbstractInput::Update()
	{
		normalizedMouseScrollDelta = Math::Vector2i(0, 0);

		mouseDeltaPos = currentPos - lastPos;
		lastPos = currentPos;

		pressedThisFrame = currentKeys & ~lastKeys;
		releasedThisFrame = lastKeys & ~currentKeys;

		lastKeys = currentKeys;
	}

	Keycode::Scancode AbstractInput::GetAnyKey()
	{
		for (size_t i = 0; i < MAX_KEYS; ++i)
		{
			if (pressedThisFrame[i])
				return static_cast<Keycode::Scancode>(i);
		}

		return Keycode::UNKNOWN;
	}
}