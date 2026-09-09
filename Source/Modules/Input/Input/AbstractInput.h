#pragma once

#include "Input.Core.hpp"

#include <bitset>

#include "Core/Math/Vector/Vector2.h"
#include "Keycodes.h"

namespace Eclipse::Input
{
#define MAX_KEYS 512
	class INPUT_API AbstractInput
	{
	public:
		bool GetKey(char aKey);
		bool GetKey(int aKey);
		bool GetKey(Keycode::Scancode aKey);

		bool GetKeyDown(char aKey);
		bool GetKeyDown(int aKey);
		bool GetKeyDown(Keycode::Scancode aKey);

		bool GetKeyUp(char aKey);
		bool GetKeyUp(int aKey);
		bool GetKeyUp(Keycode::Scancode aKey);

		bool GetAny();
		Keycode::Scancode GetAnyKey();

		bool GetMouse(int aKey);
		bool GetMouse(Keycode::Scancode aKey);

		bool GetMouseDown(int aKey);
		bool GetMouseDown(Keycode::Scancode aKey);

		bool GetMouseUp(int aKey);
		bool GetMouseUp(Keycode::Scancode aKey);


		const Math::Vector2f& GetGameMousePos();

		const Math::Vector2i& GetMousePos();
		const Math::Vector2i& GetMouseDeltaPos();

		const Math::Vector2i& GetScroll();
		const Math::Vector2i& GetNormalizedScroll();


		bool IsWindowFocused();
		bool IsMouseInside();

		void SetMousePositionEditor(const Math::Vector2f& aPosition);

	public:
		virtual void Init() = 0;

	public:
		void Update();

	protected:
		std::bitset<MAX_KEYS> currentKeys;
		std::bitset<MAX_KEYS> lastKeys;

		std::bitset<MAX_KEYS> pressedThisFrame;
		std::bitset<MAX_KEYS> releasedThisFrame;

		Math::Vector2f currentGamePos;

		Math::Vector2i currentPos;
		Math::Vector2i lastPos;
		Math::Vector2i mouseDeltaPos;

		Math::Vector2i mouseScrollDelta;
		Math::Vector2i normalizedMouseScrollDelta;

		bool mouseIsInside;
		bool windowFocused;
	};
}