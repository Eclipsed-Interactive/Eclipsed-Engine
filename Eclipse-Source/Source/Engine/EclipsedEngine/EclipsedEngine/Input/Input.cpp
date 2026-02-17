#include "EclipsedEngine/Input/Input.h"

namespace Eclipse
{
	void Input::Init()
	{
		GetInput().Init();
	}

	void Input::Update()
	{
		GetInput().Update();
	}


	bool Input::GetKey(int aKey)
	{
		return GetInput().GetKey(aKey);
	}

	bool Input::GetKey(char aKey)
	{
		return GetInput().GetKey(aKey);
	}

	bool Input::GetKey(Keycode::Scancode aKey)
	{
		return GetInput().GetKey((int)aKey);
	}



	bool Input::GetKeyDown(int aKey)
	{
		return GetInput().GetKeyDown(aKey);
	}

	bool Input::GetKeyDown(char aKey)
	{
		return GetInput().GetKeyDown(aKey);
	}

	bool Input::GetKeyDown(Keycode::Scancode aKey)
	{

		return GetInput().GetKeyDown(aKey);
	}



	bool Input::GetKeyUp(int aKey)
	{
		return GetInput().GetKeyUp(aKey);
	}

	bool Input::GetKeyUp(char aKey)
	{
		return GetInput().GetKeyUp(aKey);
	}

	bool Input::GetKeyUp(Keycode::Scancode aKey)
	{
		return GetInput().GetKeyUp(aKey);
	}

	bool Input::GetAny()
	{
		return GetInput().GetAny();
	}

	Keycode::Scancode Input::GetAnyKey()
	{
		return GetInput().GetAnyKey();
	}


	bool Input::GetMouse(int aKey)
	{
		return GetInput().GetMouse(aKey);
	}

	bool Input::GetMouse(Keycode::Scancode aKey)
	{
		return GetInput().GetMouse(aKey);
	}



	bool Input::GetMouseDown(int aKey)
	{
		return GetInput().GetMouseDown(aKey);
	}

	bool Input::GetMouseDown(Keycode::Scancode aKey)
	{
		return GetInput().GetMouseDown(aKey);
	}



	bool Input::GetMouseUp(int aKey)
	{
		return GetInput().GetMouseUp(aKey);
	}

	bool Input::GetMouseUp(Keycode::Scancode aKey)
	{
		return GetInput().GetMouseUp(aKey);
	}

	const Math::Vector2i& Input::GetGameMousePos()
	{
		return GetInput().GetGameMousePos();
	}

	const Math::Vector2i& Input::GetMousePos()
	{
		return GetInput().GetMousePos();
	}

	const Math::Vector2i& Input::GetMouseDeltaPos()
	{
		return GetInput().GetMouseDeltaPos();
	}

	const Math::Vector2i& Input::GetScroll()
	{
		return GetInput().GetScroll();
	}

	const Math::Vector2i& Input::GetNormalizedScroll()
	{
		return GetInput().GetNormalizedScroll();
	}





	bool Input::IsWindowFocused()
	{
		return GetInput().IsWindowFocused();
	}

	bool Input::IsMouseInside()
	{
		return GetInput().IsMouseInside();
	}














	// should not be in getter but rahter setters
	void Input::SetGamePosition(const Math::Vector2i& aPosition)
	{
		GetInput().SetGamePosition(aPosition);
	}
}