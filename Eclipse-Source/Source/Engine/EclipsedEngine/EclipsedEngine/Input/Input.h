#pragma once

#include "CoreEngine/Macros/defines.h"
#include "CoreEngine/Input/Input.h"

namespace Eclipse
{
	namespace Core
	{
		class Input;
	}

	ECLIPSED_API inline Core::Input& GetInput()
	{
		static Core::Input input{};
		return input;
	}

	class ECLIPSED_API Input
	{
	public:
		static bool GetKey(char aKey);
		static bool GetKey(int aKey);
		static bool GetKey(Keycode::Scancode aKey);

		static bool GetKeyDown(char aKey);
		static bool GetKeyDown(int aKey);
		static bool GetKeyDown(Keycode::Scancode aKey);

		static bool GetKeyUp(char aKey);
		static bool GetKeyUp(int aKey);
		static bool GetKeyUp(Keycode::Scancode aKey);

		static bool GetAny();
		static Keycode::Scancode GetAnyKey();

		static bool GetMouse(int aKey);
		static bool GetMouse(Keycode::Scancode aKey);

		static bool GetMouseDown(int aKey);
		static bool GetMouseDown(Keycode::Scancode aKey);

		static bool GetMouseUp(int aKey);
		static bool GetMouseUp(Keycode::Scancode aKey);


		static const Math::Vector2i& GetGameMousePos();

		static const Math::Vector2i& GetMousePos();
		static const Math::Vector2i& GetMouseDeltaPos();

		static const Math::Vector2i& GetScroll();
		static const Math::Vector2i& GetNormalizedScroll();


		static bool IsWindowFocused();
		static bool IsMouseInside();

		static void SetGamePosition(const Math::Vector2i& aPosition);


	public:
		static void Init();
		static void Update();
	};
}