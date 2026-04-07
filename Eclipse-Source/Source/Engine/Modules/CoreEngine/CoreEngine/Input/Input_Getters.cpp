#include "pch.h"

#include "Input.h"

namespace Eclipse::Core
{
	bool Input::GetKey(int aKey)
	{
		return currentKeys[aKey];
	}

	bool Input::GetKey(char aKey)
	{
		return currentKeys[aKey];
	}

	bool Input::GetKey(Keycode::Scancode aKey)
	{
		return currentKeys[(int)aKey];
	}



	bool Input::GetKeyDown(int aKey)
	{
		return pressedThisFrame[aKey];
	}

	bool Input::GetKeyDown(char aKey)
	{
		return pressedThisFrame[aKey];
	}

	bool Input::GetKeyDown(Keycode::Scancode aKey)
	{
		return pressedThisFrame[(int)aKey];
	}



	bool Input::GetKeyUp(int aKey)
	{
		return releasedThisFrame[aKey];
	}

	bool Input::GetKeyUp(char aKey)
	{
		return releasedThisFrame[aKey];
	}

	bool Input::GetKeyUp(Keycode::Scancode aKey)
	{
		return releasedThisFrame[(int)aKey];
	}

	bool Input::GetAny()
	{
		return currentKeys.any();
	}

	Keycode::Scancode Input::GetAnyKey()
	{
		for (int i = 0; i < currentKeys.size(); i++)
			if (currentKeys[i])
				return static_cast<Keycode::Scancode>(i);

		return Keycode::Scancode::NONE;
	}


	bool Input::GetMouse(int aKey)
	{
		return currentKeys[aKey];
	}

	bool Input::GetMouse(Keycode::Scancode aKey)
	{
		return currentKeys[(int)aKey];
	}



	bool Input::GetMouseDown(int aKey)
	{
		return pressedThisFrame[aKey];
	}

	bool Input::GetMouseDown(Keycode::Scancode aKey)
	{
		return pressedThisFrame[(int)aKey];
	}



	bool Input::GetMouseUp(int aKey)
	{
		return releasedThisFrame[aKey];
	}

	bool Input::GetMouseUp(Keycode::Scancode aKey)
	{
		return releasedThisFrame[(int)aKey];
	}

	const Math::Vector2f& Input::GetGameMousePos()
	{
		return currentGamePos;
	}

	const Math::Vector2i& Input::GetMousePos()
	{
		return currentPos;
	}

	const Math::Vector2i& Input::GetMouseDeltaPos()
	{
		return mouseDeltaPos;
	}

	const Math::Vector2i& Input::GetScroll()
	{
		return mouseScrollDelta;
	}

	const Math::Vector2i& Input::GetNormalizedScroll()
	{
		return normalizedMouseScrollDelta;
	}





	bool Input::IsWindowFocused()
	{
		return windowFocused;
	}

	bool Input::IsMouseInside()
	{
		return mouseIsInside;
	}














	// should not be in getter but rahter setters
	void Input::SetGamePosition(const Math::Vector2f& aPosition)
	{
		currentGamePos = aPosition;
	}
}
