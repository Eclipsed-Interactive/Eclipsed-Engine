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
}