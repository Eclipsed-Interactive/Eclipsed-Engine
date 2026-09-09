#include "AbstractInput.h"

namespace Eclipse::Input
{
	void AbstractInput::Update()
	{
		// Mouse delta
		//mouseScrollDelta = Math::Vector2i(0, 0);
		normalizedMouseScrollDelta = Math::Vector2i(0, 0);

		// Mouse pos 
		mouseDeltaPos = currentPos - lastPos;
		lastPos = currentPos;

		// Buttons (keyboard and mouse buttons)
		pressedThisFrame = currentKeys & ~lastKeys;
		releasedThisFrame = lastKeys & ~currentKeys;

		lastKeys = currentKeys;
	}
}