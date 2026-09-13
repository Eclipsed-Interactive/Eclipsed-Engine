#pragma once

#include "Input.Core.hpp"

#include <bitset>

#include "Core/Math/Vector/Vector2.h"
#include "Keycodes.h"

#include "AbstractInput.h"

namespace Eclipse::Input
{
	class INPUT_API Input
	{
	public:
		static void Init(AbstractInput* inputInstance, void* imguiCtx);
		static void Update();

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


		static const Math::Vector2i& GetMousePos();
		static const Math::Vector2i& GetMouseDeltaPos();

		static const Math::Vector2i& GetScroll();
		static const Math::Vector2i& GetNormalizedScroll();


		static bool IsWindowFocused();
		static bool IsMouseInside();

	public:
		
	private:
		static AbstractInput* input;

#ifdef ECL_EDITOR
		static std::vector<std::string> gameViewWindows;

	public:
		static Math::Vector2ui GetActiveGameViewMousePos();

		static void AddGameViewWindow(const std::string& id)
		{
			gameViewWindows.push_back(id);
		}
#endif
	};
}